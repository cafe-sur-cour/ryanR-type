/*
** EPITECH PROJECT, 2026
** ryanR-type
** File description:
** Lobby
*/

#include <set>
#include <map>
#include <utility>
#include <memory>
#include <tuple>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <filesystem>   // NOLINT(build/c++17)
#include <nlohmann/json.hpp>

#include "Lobby.hpp"
#include "constants.hpp"
#include "../libs/Network/Unix/ServerNetwork.hpp"
#include "../common/components/tags/PlayerTag.hpp"
#include "../common/components/permanent/GameZoneComponent.hpp"
#include "../common/ECS/entity/Entity.hpp"
#include "../common/ECS/entity/registry/Registry.hpp"

#include "initResourcesManager/initResourcesManager.hpp"
#include "gsm/states/scenes/Boot/BootState.hpp"
#include "../common/debug.hpp"
#include "../../common/systems/systemManager/SystemManager.hpp"
#include "../../common/Prefab/entityPrefabManager/EntityPrefabManager.hpp"
#include "../common/components/permanent/ScriptingComponent.hpp"
#include "../common/components/permanent/ScoreComponent.hpp"
#include "../common/GameRules.hpp"

rserv::Lobby::Lobby(std::shared_ptr<net::INetwork> network,
    std::vector<std::tuple<uint8_t, std::shared_ptr<net::INetworkEndpoint>,
    std::string>> lobbyPlayerInfo,
    std::string lobbyCode, bool debug, int64_t tps) {
    this->_network = network;
    this->_clients = lobbyPlayerInfo;
    this->_lobbyCode = lobbyCode;
    this->_isDebug = debug;
    this->_tps = tps;

    this->_clientsReady = std::map<uint8_t, bool>();
    this->_clientToEntity = std::map<uint8_t, ecs::Entity>();

    this->_packet = nullptr;
    this->_sequenceNumber = 0;
    this->_resourceManager = nullptr;
    this->_gameStarted = false;
    this->_playerEntitiesCreated = false;
    this->_eventQueue = std::make_shared<std::queue<std::tuple<uint8_t,
        constants::EventType, double>>>();
    this->_lastGameStateTime = std::chrono::steady_clock::now();
    this->_statusUpdateTimer = 0.0f;
    this->_running = true;
    this->_gsm = nullptr;

    this->_convertFunctions = {
        std::bind(&rserv::Lobby::convertTransformComponent, this,
            std::placeholders::_1, std::placeholders::_2),
        std::bind(&rserv::Lobby::convertHealthComponent, this,
            std::placeholders::_1, std::placeholders::_2),
        std::bind(&rserv::Lobby::convertScoreComponent, this,
            std::placeholders::_1, std::placeholders::_2),
        std::bind(&rserv::Lobby::convertChargedShotComponent, this,
            std::placeholders::_1, std::placeholders::_2),
        std::bind(&rserv::Lobby::convertAnimationStateComponent, this,
            std::placeholders::_1, std::placeholders::_2)
    };
}

rserv::Lobby::~Lobby() {
    this->stop();

    if (!this->_resourceManager) {
        return;
    }

    if (!this->_resourceManager->has<ecs::Registry>()) {
        debug::Debug::printDebug(this->getIsDebug(),
            "[SERVER] Registry not found, cannot process WHOAMI",
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return;
    }

    auto registry = this->_resourceManager->get<ecs::Registry>();

    if (registry != nullptr) {
        auto scriptingView = registry->view<ecs::ScriptingComponent>();
        for (auto entityId : scriptingView) {
            auto scriptingComp = registry->getComponent<ecs::ScriptingComponent>(
                entityId
            );
            if (scriptingComp) {
                scriptingComp->clearLuaReferences();
            }
        }
    }
}


/* Client Info Getter */
std::vector<uint8_t> rserv::Lobby::getConnectedClients() const {
    std::vector<uint8_t> clientIds;
    std::lock_guard<std::mutex> lock(_clientsMutex);
    for (const auto &client : this->_clients) {
        clientIds.push_back(std::get<0>(client));
    }
    return clientIds;
}

std::vector<std::tuple<uint8_t, std::string>> rserv::Lobby::getConnectedClientDetails() const {
    std::vector<std::tuple<uint8_t, std::string>> clientDetails;
    std::lock_guard<std::mutex> lock(_clientsMutex);
    for (const auto &client : this->_clients) {
        clientDetails.emplace_back(std::get<0>(client), std::get<2>(client));
    }
    return clientDetails;
}

std::vector<std::shared_ptr<net::INetworkEndpoint>>
    rserv::Lobby::getConnectedClientEndpoints() const {
    std::vector<std::shared_ptr<net::INetworkEndpoint>> endpoints;
    std::lock_guard<std::mutex> lock(_clientsMutex);
    for (const auto &client : this->_clients) {
        endpoints.push_back(std::get<1>(client));
    }
    return endpoints;
}

size_t rserv::Lobby::getClientCount() const {
    std::lock_guard<std::mutex> lock(_clientsMutex);
    return this->_clients.size();
}

bool rserv::Lobby::isRunning() const {
    return this->_running.load(std::memory_order_acquire);
}

void rserv::Lobby::addClient(
    std::tuple<uint8_t,
    std::shared_ptr<net::INetworkEndpoint>,
    std::string> client
) {
    std::lock_guard<std::mutex> lock(_clientsMutex);
    this->_clients.push_back(client);
}

void rserv::Lobby::removeClient(uint8_t clientId) {
    std::lock_guard<std::mutex> lock(_clientsMutex);
    auto it = std::remove_if(_clients.begin(), _clients.end(),
        [clientId](const std::tuple<
            uint8_t, std::shared_ptr<net::INetworkEndpoint>, std::string>& client) {
            return std::get<0>(client) == clientId;
        });
    _clients.erase(it, _clients.end());

    _clientsReady.erase(clientId);
    _clientToEntity.erase(clientId);
    _deltaTracker.clearClientCache(clientId);
}

void rserv::Lobby::createPlayerEntityForClient(uint8_t clientId) {
    if (this->_resourceManager == nullptr) {
        debug::Debug::printDebug(this->getIsDebug(),
            "[LOBBY] Cannot create player entity: resource manager not set",
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return;
    }

    auto prefabMgr = this->_resourceManager->get<EntityPrefabManager>();
    auto registry = this->_resourceManager->get<ecs::Registry>();

    if (prefabMgr == nullptr || registry == nullptr) {
        debug::Debug::printDebug(this->getIsDebug(),
            "[LOBBY] Cannot create player entity: missing required components",
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return;
    }

    if (this->_clientToEntity.find(clientId) != this->_clientToEntity.end()) {
        debug::Debug::printDebug(this->getIsDebug(),
            "[LOBBY] Player entity already exists for client " +
                std::to_string(static_cast<int>(clientId)),
            debug::debugType::NETWORK, debug::debugLevel::INFO);
        return;
    }

    if (this->_resourceManager->has<ecs::ServerInputProvider>()) {
        auto inputProvider = this->_resourceManager->get<ecs::ServerInputProvider>();
        if (inputProvider) {
            inputProvider->registerClient(clientId);
            debug::Debug::printDebug(this->getIsDebug(),
                "[LOBBY] Registered client " + std::to_string(static_cast<int>(clientId)) +
                " with input provider",
                debug::debugType::NETWORK, debug::debugLevel::INFO);
        }
    }

    std::string playerString = constants::PLAYER_PREFAB_NAME;
    ecs::Entity playerEntity = prefabMgr->createEntityFromPrefab(
        playerString,
        registry,
        ecs::EntityCreationContext::forServer()
    );

    this->_clientToEntity[clientId] = playerEntity;

    if (this->_resourceManager->has<ecs::ServerInputProvider>()) {
        auto inputProvider = this->_resourceManager->get<ecs::ServerInputProvider>();
        if (inputProvider) {
            inputProvider->registerEntityForClient(playerEntity, clientId);
            debug::Debug::printDebug(this->getIsDebug(),
                "[LOBBY] Registered entity " + std::to_string(playerEntity) +
                " for client " + std::to_string(static_cast<int>(clientId)),
                debug::debugType::NETWORK, debug::debugLevel::INFO);
        }
    }

    debug::Debug::printDebug(this->getIsDebug(),
        "[LOBBY] Created player entity " + std::to_string(playerEntity) +
        " for client " + std::to_string(static_cast<int>(clientId)),
        debug::debugType::NETWORK, debug::debugLevel::INFO);
}

void rserv::Lobby::syncExistingEntitiesToClient(
    std::shared_ptr<net::INetworkEndpoint> clientEndpoint
) {
    if (!this->_resourceManager) {
        return;
    }

    auto registry = this->_resourceManager->get<ecs::Registry>();
    auto prefabMgr = this->_resourceManager->get<EntityPrefabManager>();

    if (!registry || !prefabMgr) {
        return;
    }

    for (const auto &[otherClientId, playerEntity] : this->_clientToEntity) {
        std::vector<uint64_t> spawnPayload = this->spawnPacket(playerEntity, "player");
        std::vector<uint8_t> spawnPacketData = this->_packet->pack(
            constants::ID_SERVER,
            this->_sequenceNumber,
            constants::PACKET_SPAWN,
            spawnPayload
        );
        this->_network->sendTo(*clientEndpoint, spawnPacketData);
        this->_sequenceNumber++;

        debug::Debug::printDebug(this->getIsDebug(),
            "[LOBBY] Sent spawn packet for existing player entity " +
                std::to_string(playerEntity) +
            " to new client",
            debug::debugType::NETWORK, debug::debugLevel::INFO);
    }
}

std::shared_ptr<net::INetwork> rserv::Lobby::getNetwork() const {
    return this->_network;
}

std::string rserv::Lobby::getLobbyCode() const {
    return this->_lobbyCode;
}

std::string rserv::Lobby::getGameState() const {
    if (this->_gsm) {
        return this->_gsm->getCurrentStateName();
    }
    return "Not Started";
}

std::string rserv::Lobby::getGameRules() const {
    if (!this->_resourceManager->has<GameRules>()) {
        return "Rules not available";
    }

    auto gameRules = this->_resourceManager->get<GameRules>();
    std::string gamemodeStr;
    std::string difficultyStr;
    std::string crossfireStr;

    switch (gameRules->getGamemode()) {
        case GameRulesNS::Gamemode::CLASSIC:
            gamemodeStr = "Classic";
            break;
        case GameRulesNS::Gamemode::INFINITE_MODE:
            gamemodeStr = "Infinite Mode";
            break;
        default:
            gamemodeStr = "Unknown";
            break;
    }

    switch (gameRules->getDifficulty()) {
        case GameRulesNS::Difficulty::EASY:
            difficultyStr = "Easy";
            break;
        case GameRulesNS::Difficulty::NORMAL:
            difficultyStr = "Normal";
            break;
        case GameRulesNS::Difficulty::HARD:
            difficultyStr = "Hard";
            break;
        default:
            difficultyStr = "Unknown";
            break;
    }

    crossfireStr = gameRules->getCrossfire() ? "Enabled" : "Disabled";

    return "Mode: " + gamemodeStr + " | Difficulty: " + difficultyStr +
        " | Crossfire: " + crossfireStr;
}


/* Event Queue hadling */
std::shared_ptr<std::queue<std::tuple<uint8_t, constants::EventType, double>>>
    rserv::Lobby::getEventQueue() {
    return this->_eventQueue;
}

bool rserv::Lobby::hasEvents() const {
    std::lock_guard<std::mutex> lock(_eventMutex);
    return !this->_eventQueue->empty();
}


/* Is Debug */
void rserv::Lobby::setIsDebug(bool debug) {
    this->_isDebug = debug;
}

bool rserv::Lobby::getIsDebug() const {
    return this->_isDebug;
}

std::shared_ptr<ResourceManager> rserv::Lobby::getResourceManager() const {
    return this->_resourceManager;
}

const std::map<uint8_t, ecs::Entity>& rserv::Lobby::getClientToEntity() const {
    return this->_clientToEntity;
}


/* Packet Processing */

void rserv::Lobby::processIncomingPackets() {
    std::pair<std::shared_ptr<net::INetworkEndpoint>, std::vector<uint8_t>> received;
    {
        std::lock_guard<std::mutex> lock(_packetMutex);
        if (_incomingPackets.empty()) {
            return;
        }
        received = _incomingPackets.front();
        _incomingPackets.pop();
    }

    if (received.second.empty()) {
        return;
    }

    if (!this->_packet) {
        std::cerr << "[SERVER] Warning: Packet manager not initialized" << std::endl;
        return;
    }

    this->_packet->unpack(received.second);
    uint8_t clientId = this->_packet->getIdClient();

    if (this->_packet->getType() == constants::PACKET_EVENT) {
        this->processEvents(clientId);
    } else if (this->_packet->getType() == constants::PACKET_WHOAMI) {
        this->processWhoAmI(clientId);
    } else if (this->_packet->getType() == constants::PACKET_LEAVE_LOBBY) {
        this->processDisconnections(clientId);
    } else {
        debug::Debug::printDebug(this->getIsDebug(),
            "[SERVER] Packet received of type "
            + std::to_string(static_cast<int>(this->_packet->getType()))
            + " from client: " + std::to_string(clientId),
            debug::debugType::NETWORK, debug::debugLevel::INFO);
    }

    this->_packet->reset();
}

bool rserv::Lobby::processDisconnections(uint8_t idClient) {
    std::lock_guard<std::mutex> lock(_clientsMutex);

    for (auto &client : this->_clients) {
        if (std::get<0>(client) == idClient) {
            if (this->_running.load(std::memory_order_acquire)) {
                this->ackLeaveLobbyPacket(*std::get<1>(client), true);
            }
            this->_clients.erase(
                std::remove(this->_clients.begin(), this->_clients.end(), client),
                this->_clients.end());
            this->_clientToEntity.erase(idClient);
            this->_clientsReady.erase(idClient);
            debug::Debug::printDebug(this->getIsDebug(),
                "Client " + std::to_string(idClient)
                + " disconnected and removed from the lobby",
                debug::debugType::NETWORK, debug::debugLevel::INFO);
            return true;
        }
    }
    return false;
}

bool rserv::Lobby::processEvents(uint8_t idClient) {
    debug::Debug::printDebug(this->getIsDebug(),
        "[SERVER] Processing event packet from client: "
        + std::to_string(idClient),
        debug::debugType::NETWORK, debug::debugLevel::INFO);

    auto payload = this->_packet->getPayload();
    if (payload.size() < 2) {
        debug::Debug::printDebug(this->getIsDebug(),
            "[SERVER] Invalid event packet payload size: " + std::to_string(payload.size()),
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return false;
    }

    constants::EventType eventType =
        static_cast<constants::EventType>(payload.at(0));

    uint64_t param1Bits = payload.at(1);
    double param1;
    std::memcpy(&param1, &param1Bits, sizeof(double));

    {
        std::lock_guard<std::mutex> lock(_eventMutex);
        this->_eventQueue->push(std::tuple(idClient, eventType, param1));
    }
    return true;
}

bool rserv::Lobby::processWhoAmI(uint8_t idClient) {
    debug::Debug::printDebug(this->getIsDebug(),
        "[SERVER] Processing WHOAMI request from client: "
        + std::to_string(idClient),
        debug::debugType::NETWORK, debug::debugLevel::INFO);

    if (!this->_resourceManager->has<ecs::Registry>()) {
        debug::Debug::printDebug(this->getIsDebug(),
            "[SERVER] Registry not found, cannot process WHOAMI",
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return false;
    }

    auto registry = this->_resourceManager->get<ecs::Registry>();
    auto it = this->_clientToEntity.find(idClient);

    if (it == this->_clientToEntity.end()) {
        debug::Debug::printDebug(this->getIsDebug(),
            "[SERVER] No player entity found for client " + std::to_string(idClient) +
            ", creating one now",
            debug::debugType::NETWORK, debug::debugLevel::INFO);

        this->createPlayerEntityForClient(idClient);
        it = this->_clientToEntity.find(idClient);

        if (it == this->_clientToEntity.end()) {
            debug::Debug::printDebug(this->getIsDebug(),
                "[SERVER] Failed to create player entity for client "
                    + std::to_string(idClient),
                debug::debugType::NETWORK, debug::debugLevel::ERROR);
            return false;
        }
    }

    ecs::Entity playerEntity = it->second;

    std::vector<uint64_t> payload;
    payload.push_back(static_cast<uint64_t>(playerEntity));

    std::shared_ptr<net::INetworkEndpoint> clientEndpoint;
    for (const auto &client : this->_clients) {
        if (std::get<0>(client) == idClient) {
            clientEndpoint = std::get<1>(client);
            break;
        }
    }

    if (!clientEndpoint) {
        debug::Debug::printDebug(this->getIsDebug(),
            "[SERVER] No endpoint found for client " + std::to_string(idClient),
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return false;
    }

    std::vector<uint8_t> packet = this->_packet->pack(constants::ID_SERVER,
        this->_sequenceNumber, constants::PACKET_WHOAMI, payload);

    if (!this->_network->sendTo(*clientEndpoint, packet)) {
        debug::Debug::printDebug(this->getIsDebug(),
            "[SERVER] Failed to send WHOAMI response to client " + std::to_string(idClient),
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return false;
    }

    this->_sequenceNumber++;
    debug::Debug::printDebug(this->getIsDebug(),
        "[SERVER] Sent WHOAMI response to client " + std::to_string(idClient)
        + " with entity ID " + std::to_string(playerEntity),
        debug::debugType::NETWORK, debug::debugLevel::INFO);

    return true;
}





/* Packet Sending */

bool rserv::Lobby::gameStatePacket() {
    if (this->_resourceManager == nullptr) {
        std::cerr << "Ressource manager not set in lobby, cannot send game state packet"
            << std::endl;
        exit(84);
    }
    if (!this->_resourceManager->has<ecs::Registry>()) {
        return false;
    }

    auto registry = this->_resourceManager->get<ecs::Registry>();
    auto entityView = registry->view<ecs::TransformComponent>();

    static int cleanupCounter = 0;
    if (++cleanupCounter >= 1200) {
        cleanupCounter = 0;
        std::set<uint32_t> aliveEntityIds;
        for (auto entityId : entityView) {
            aliveEntityIds.insert(static_cast<uint32_t>(entityId));
        }
        this->_deltaTracker.clearDeadEntities(aliveEntityIds);
    }

    struct EntityData {
        uint32_t entityId;
        std::vector<uint64_t> componentData;
        EntitySnapshot snapshot;
        bool isGameZone = false;
    };

    std::vector<EntityData> serializedEntities;
    serializedEntities.reserve(256);
    for (auto entityId : entityView) {
        EntityData entityData;
        entityData.entityId = static_cast<uint32_t>(entityId);
        entityData.componentData.reserve(64);
        entityData.isGameZone = registry->hasComponent<ecs::GameZoneComponent>(entityId);
        for (const auto& func : this->_convertFunctions) {
            std::vector<uint64_t> compData = func(registry, entityId);
            entityData.componentData.insert(entityData.componentData.end(),
                compData.begin(), compData.end());
        }

        entityData.snapshot = ComponentSerializer::createSnapshotFromComponents(
            entityData.entityId, entityData.componentData
        );

        serializedEntities.push_back(std::move(entityData));
    }

    std::stable_sort(serializedEntities.begin(), serializedEntities.end(),
        [](const EntityData& a, const EntityData& b) {
            return a.isGameZone > b.isGameZone;
        });

    constexpr size_t MAX_BATCH_SIZE = 20;
    constexpr size_t MAX_PACKET_SIZE = 1400;

    std::vector<uint8_t> clientIds;
    {
        std::lock_guard<std::mutex> lock(_clientsMutex);
        for (const auto& client : this->_clients) {
            clientIds.push_back(std::get<0>(client));
        }
    }

    for (uint8_t clientId : clientIds) {
        std::shared_ptr<net::INetworkEndpoint> clientEndpoint = nullptr;
        {
            std::lock_guard<std::mutex> lock(_clientsMutex);
            for (const auto& client : this->_clients) {
                if (std::get<0>(client) == clientId) {
                    clientEndpoint = std::get<1>(client);
                    break;
                }
            }
        }

        if (!clientEndpoint) {
            continue;
        }

        std::vector<std::vector<uint64_t>> batchedEntities;
        batchedEntities.reserve(MAX_BATCH_SIZE);
        size_t estimatedSize = 2;

        for (const auto& entityData : serializedEntities) {
            std::vector<uint64_t> delta = this->_deltaTracker.createEntityDelta(
                clientId, entityData.entityId, entityData.snapshot);
            if (delta.empty()) {
                continue;
            }

            std::vector<uint64_t> payload;
            payload.reserve(entityData.componentData.size() + 1);
            payload.push_back(entityData.entityId);
            payload.insert(payload.end(), entityData.componentData.begin(),
                entityData.componentData.end());

            size_t entityEstimate = 3 + payload.size() * 2;
            if (batchedEntities.size() >= MAX_BATCH_SIZE ||
                (estimatedSize + entityEstimate > MAX_PACKET_SIZE &&
                !batchedEntities.empty())) {
                std::vector<uint8_t> packet = this->_packet->packBatchedGameState(
                    constants::ID_SERVER,
                    this->_sequenceNumber++,
                    batchedEntities
                );

                if (!this->_network->sendTo(*clientEndpoint, packet)) {
                    debug::Debug::printDebug(this->getIsDebug(),
                        "[SERVER NETWORK] Failed to send batched game state to client " +
                        std::to_string(static_cast<int>(clientId)),
                        debug::debugType::NETWORK, debug::debugLevel::ERROR);
                    this->processDisconnections(clientId);
                    break;
                }
                batchedEntities.clear();
                estimatedSize = 2;
            }

            batchedEntities.push_back(std::move(payload));
            estimatedSize += entityEstimate;
        }

        if (!batchedEntities.empty()) {
            std::vector<uint8_t> packet = this->_packet->packBatchedGameState(
                constants::ID_SERVER,
                this->_sequenceNumber++,
                batchedEntities
            );

            if (!this->_network->sendTo(*clientEndpoint, packet)) {
                debug::Debug::printDebug(this->getIsDebug(),
                    "[SERVER NETWORK] Failed to send batched game state packet to client " +
                    std::to_string(static_cast<int>(clientId)),
                    debug::debugType::NETWORK, debug::debugLevel::ERROR);
                this->processDisconnections(clientId);
            }
        }
    }
    return true;
}

bool rserv::Lobby::endGamePacket(bool isWin) {
    if (this->_resourceManager && this->_resourceManager->has<ecs::Registry>()) {
        auto registry = this->_resourceManager->get<ecs::Registry>();
        const std::string filepath = constants::SCORES_JSON_PATH;
        nlohmann::json scores;
        std::ifstream scoresFile(filepath);
        if (scoresFile.is_open()) {
            try {
                scoresFile >> scores;
            } catch (const std::exception&) {
                scores = nlohmann::json::object();
            }
            scoresFile.close();
        } else {
            scores = nlohmann::json::object();
        }

        for (const auto& client : this->_clients) {
            uint8_t clientId = std::get<0>(client);
            std::string username = std::get<2>(client);
            auto it = this->_clientToEntity.find(clientId);
            if (it == this->_clientToEntity.end()) {
                debug::Debug::printDebug(this->getIsDebug(),
                    "[SERVER] No entity found for client " + std::to_string(clientId),
                    debug::debugType::NETWORK, debug::debugLevel::WARNING);
                continue;
            }
            ecs::Entity entity = it->second;
            if (!registry->hasComponent<ecs::ScoreComponent>(entity)) {
                debug::Debug::printDebug(this->getIsDebug(),
                    "[SERVER] ScoreComponent not found for entity " + std::to_string(entity),
                    debug::debugType::NETWORK, debug::debugLevel::WARNING);
                continue;
            }
            auto scoreComp = registry->getComponent<ecs::ScoreComponent>(entity);
            int score = scoreComp->getScore();
            if (!scores.contains(username)) {
                scores[username] = {{"scores", nlohmann::json::array()}};
            }
            scores[username]["scores"].push_back(score);
            debug::Debug::printDebug(this->getIsDebug(),
                "[SERVER] Saved score " + std::to_string(score) + " for user " + username,
                debug::debugType::NETWORK, debug::debugLevel::INFO);
        }

        std::filesystem::create_directories(std::filesystem::path(filepath).parent_path());
        std::ofstream outFile(filepath);
        if (outFile.is_open()) {
            outFile << scores.dump(4);
            outFile.close();
            debug::Debug::printDebug(this->getIsDebug(),
                "[SERVER] Successfully saved scores to file",
                debug::debugType::NETWORK, debug::debugLevel::INFO);
        } else {
            debug::Debug::printDebug(this->getIsDebug(),
                "[SERVER] Failed to save scores to file",
                debug::debugType::NETWORK, debug::debugLevel::ERROR);
        }
    } else {
        debug::Debug::printDebug(this->getIsDebug(),
            "[SERVER] Registry not found, cannot save scores",
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
    }

    std::vector<uint64_t> payload;
    payload.push_back(isWin ? 1 : 0);
    std::vector<uint8_t> packet = this->_packet->pack(constants::ID_SERVER,
        this->_sequenceNumber, constants::PACKET_END_GAME, payload);

    if (!this->_network->broadcast(this->getConnectedClientEndpoints(), packet)) {
        debug::Debug::printDebug(this->getIsDebug(),
            "[SERVER NETWORK] Failed to broadcast end game packet",
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return false;
    }
    this->_sequenceNumber++;
    return true;
}

bool rserv::Lobby::levelCompletePacket() {
    std::vector<uint64_t> payload;
    std::vector<uint8_t> packet = this->_packet->pack(constants::ID_SERVER,
        this->_sequenceNumber, constants::PACKET_LEVEL_COMPLETE, payload);

    if (!this->_network->broadcast(this->getConnectedClientEndpoints(), packet)) {
        debug::Debug::printDebug(this->getIsDebug(),
            "[SERVER NETWORK] Failed to broadcast level complete packet",
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return false;
    }
    this->_sequenceNumber++;
    return true;
}

bool rserv::Lobby::nextLevelPacket() {
    std::vector<uint64_t> payload;
    std::vector<uint8_t> packet = this->_packet->pack(constants::ID_SERVER,
        this->_sequenceNumber, constants::PACKET_NEXT_LEVEL, payload);

    if (!this->_network->broadcast(this->getConnectedClientEndpoints(), packet)) {
        debug::Debug::printDebug(this->getIsDebug(),
            "[SERVER NETWORK] Failed to broadcast next level packet",
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return false;
    }
    this->_sequenceNumber++;
    return true;
}

bool rserv::Lobby::gameRulesPacket() {
    if (!this->_network) {
        debug::Debug::printDebug(this->getIsDebug(),
            "[LOBBY] Warning: Network not initialized",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }

    if (!this->_resourceManager->has<GameRules>()) {
        debug::Debug::printDebug(this->getIsDebug(),
            "[LOBBY] GameRules not found in resource manager",
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return false;
    }

    auto gameRules = this->_resourceManager->get<GameRules>();
    GameRulesNS::Gamemode gamemode = gameRules->getGamemode();
    GameRulesNS::Difficulty difficulty = gameRules->getDifficulty();
    bool crossfire = gameRules->getCrossfire();

    std::vector<uint64_t> payload;
    payload.push_back(static_cast<uint64_t>(gamemode));
    payload.push_back(static_cast<uint64_t>(difficulty));
    payload.push_back(static_cast<uint64_t>(crossfire ? 1 : 0));

    std::vector<uint8_t> packet = this->_packet->pack(
        constants::ID_SERVER,
        this->_sequenceNumber,
        constants::PACKET_GAME_RULES,
        payload
    );

    if (!this->_network->broadcast(this->getConnectedClientEndpoints(), packet)) {
        debug::Debug::printDebug(this->getIsDebug(),
            "[LOBBY] Failed to broadcast game rules packet",
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return false;
    }

    debug::Debug::printDebug(this->getIsDebug(),
        "[LOBBY] Successfully sent gameRules packet with gamemode: " +
        std::to_string(static_cast<int>(gamemode)) + " difficulty: " +
        std::to_string(static_cast<int>(difficulty)) + " and crossfire: " +
        (crossfire ? "ON" : "OFF"),
        debug::debugType::NETWORK, debug::debugLevel::INFO);

    this->_sequenceNumber++;
    return true;
}

std::vector<uint64_t> rserv::Lobby::spawnPacket(size_t entityId,
    const std::string prefabName) {
    std::vector<uint64_t> payload;

    payload.push_back(static_cast<uint64_t>(entityId));
    for (const auto &c : prefabName) {
        payload.push_back(static_cast<uint64_t>(c));
    }
    payload.push_back(static_cast<uint64_t>(constants::END_OFSTRING_ST));
    payload.push_back(static_cast<uint64_t>(constants::END_OFSTRING_ND));
    payload.push_back(static_cast<uint64_t>(constants::END_OFSTRING_TRD));
    return payload;
}

std::vector<uint64_t> rserv::Lobby::deathPacket(size_t entityId) {
    std::vector<uint64_t> payload;

    payload.push_back(static_cast<uint64_t>(entityId));
    return payload;
}

bool rserv::Lobby::serverStatusPacket() {
    std::vector<std::tuple<
        uint8_t, std::shared_ptr<net::INetworkEndpoint>, bool>> clientSnapshot;
    size_t connectedClients = 0;
    size_t readyClients = 0;

    {
        std::lock_guard<std::mutex> lock(_clientsMutex);
        connectedClients = this->_clients.size();

        for (const auto &client : this->_clients) {
            uint8_t clientId = std::get<0>(client);
            auto it = this->_clientsReady.find(clientId);
            bool isReady = it != this->_clientsReady.end() && it->second;
            if (isReady) {
                readyClients++;
            }
            clientSnapshot.push_back(std::make_tuple(clientId, std::get<1>(client), isReady));
        }
    }

    for (const auto &[clientId, endpoint, isClientReady] : clientSnapshot) {
        std::vector<uint64_t> payload = {
            static_cast<uint64_t>(connectedClients),
            static_cast<uint64_t>(readyClients),
            static_cast<uint64_t>(clientId),
            static_cast<uint64_t>(isClientReady ? 1 : 0)
        };

        std::vector<uint8_t> packet = this->_packet->pack(
            constants::ID_SERVER,
            this->_sequenceNumber,
            constants::PACKET_SERVER_STATUS,
            payload
        );

        if (!this->_network->sendTo(*endpoint, packet)) {
            debug::Debug::printDebug(this->getIsDebug(),
                "[LOBBY NETWORK] Failed to send server status packet to client " +
                std::to_string(static_cast<int>(clientId)),
                debug::debugType::NETWORK, debug::debugLevel::ERROR);
            return false;
        }
    }

    this->_sequenceNumber++;
    debug::Debug::printDebug(this->getIsDebug(),
        "[LOBBY] Sent server status packets to all lobby clients",
        debug::debugType::NETWORK, debug::debugLevel::INFO);
    return true;
}

/* Game Boolean */
bool rserv::Lobby::isGameStarted() const {
    return this->_gameStarted;
}

bool rserv::Lobby::allClientsReady() const {
    std::lock_guard<std::mutex> lock(_clientsMutex);
    if (this->_clients.empty()) {
        return false;
    }

    for (const auto &client : this->_clients) {
        uint8_t clientId = std::get<0>(client);
        auto it = this->_clientsReady.find(clientId);
        if (it == this->_clientsReady.end() || !it->second) {
            return false;
        }
    }
    return true;
}


/* Sequence Number & Packet Manager Getter and setter */
uint32_t rserv::Lobby::getSequenceNumber() const {
    return this->_sequenceNumber;
}

std::shared_ptr<pm::IPacketManager> rserv::Lobby::getPacketManager() const {
    return this->_packet;
}

void rserv::Lobby::setPacketManager(std::shared_ptr<pm::IPacketManager> packet) {
    this->_packet = packet;
}

void rserv::Lobby::incrementSequenceNumber() {
    this->_sequenceNumber++;
}

void rserv::Lobby::setResourceManager(std::shared_ptr<ResourceManager> resourceManager) {
    if (!resourceManager) {
        this->_resourceManager = resourceManager;
        return;
    }
    this->_resourceManager = resourceManager;

    if (this->_resourceManager->has<gsm::GameStateMachine>()) {
        auto gsm = this->_resourceManager->get<gsm::GameStateMachine>();
        std::shared_ptr<gsm::BootState> bootState = std::make_shared<gsm::BootState>(
            gsm,
            this->_resourceManager
        );
        gsm->changeState(bootState);
        this->_gsm = gsm;
        this->_gameStarted = true;
    } else {
        std::cerr << "Warning: Resource manager does not contain GSM" << std::endl;
    }
}

void rserv::Lobby::clearEntityDeltaCache(uint8_t clientId, uint32_t entityId) {
    this->_deltaTracker.clearEntityCache(clientId, entityId);
}

void rserv::Lobby::clearDeltaTrackerCaches() {
    this->_deltaTracker.clearAllCaches();
}

/* Game loops */
void rserv::Lobby::processLobbyEvents() {
    if (!this->_resourceManager) {
        return;
    }

    {
        std::lock_guard<std::mutex> lock(_eventMutex);
        if (_eventQueue->empty()) {
            return;
        }
    }

    if (!this->_resourceManager->has<ecs::ServerInputProvider>()) {
        return;
    }

    auto inputProvider = this->_resourceManager->get<ecs::ServerInputProvider>();
    if (!inputProvider) {
        return;
    }
    {
        std::lock_guard<std::mutex> lock(_eventMutex);
        auto eventQueue = this->getEventQueue();
        while (!eventQueue->empty()) {
            auto event = eventQueue->front();
            eventQueue->pop();
            uint8_t clientId = std::get<0>(event);
            constants::EventType eventType = std::get<1>(event);
            double param1 = std::get<2>(event);
            inputProvider->updateInputFromEvent
                (clientId, eventType, static_cast<float>(param1));
        }
    }
}

void rserv::Lobby::createPlayerEntities() {
    if (this->_resourceManager == nullptr) {
        debug::Debug::printDebug(this->getIsDebug(),
            "[LOBBY] Cannot create player entities: resource manager not set",
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return;
    }

    auto prefabMgr = this->_resourceManager->get<EntityPrefabManager>();
    auto registry = this->_resourceManager->get<ecs::Registry>();

    if (prefabMgr == nullptr || registry == nullptr) {
        debug::Debug::printDebug(this->getIsDebug(),
            "[LOBBY] Cannot create player entities: missing required components",
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return;
    }

    std::vector<uint8_t> clientIds;
    {
        std::lock_guard<std::mutex> lock(_clientsMutex);
        for (const auto &client : this->_clients) {
            clientIds.push_back(std::get<0>(client));
        }
    }

    if (this->_resourceManager->has<ecs::ServerInputProvider>()) {
        auto inputProvider = this->_resourceManager->get<ecs::ServerInputProvider>();
        if (inputProvider) {
            for (uint8_t clientId : clientIds) {
                inputProvider->registerClient(clientId);
                debug::Debug::printDebug(this->getIsDebug(),
                    "[LOBBY] Registered client " +
                    std::to_string(static_cast<int>(clientId)) +
                    " with input provider",
                    debug::debugType::NETWORK, debug::debugLevel::INFO);
            }
        }
    }

    std::string playerString = "player";
    for (uint8_t clientId : clientIds) {
        ecs::Entity playerEntity = prefabMgr->createEntityFromPrefab(
            playerString,
            registry,
            ecs::EntityCreationContext::forServer()
        );

        this->_clientToEntity[clientId] = playerEntity;

        if (this->_resourceManager->has<ecs::ServerInputProvider>()) {
            auto inputProvider = this->_resourceManager->get<ecs::ServerInputProvider>();
            if (inputProvider) {
                inputProvider->registerEntityForClient(playerEntity, clientId);
            }
        }

        debug::Debug::printDebug(this->getIsDebug(),
            "[LOBBY] Created player entity " + std::to_string(playerEntity) +
            " for client " + std::to_string(static_cast<int>(clientId)),
            debug::debugType::NETWORK, debug::debugLevel::INFO);
    }
}

void rserv::Lobby::stop() {
    this->_running.store(false, std::memory_order_release);
    if (this->_networkThread.joinable()) {
        this->_networkThread.join();
    }
    if (this->_gameThread.joinable()) {
        this->_gameThread.join();
    }
    if (this->_resourceManager) {
        auto registry = this->_resourceManager->get<ecs::Registry>();
        if (registry) {
            registry->clearAllEntities();
            debug::Debug::printDebug(this->getIsDebug(),
                "[LOBBY] Cleared all entities from registry",
                debug::debugType::NETWORK, debug::debugLevel::INFO);
        }

        if (this->_gsm) {
            this->_gsm = nullptr;
        }

        this->_clientToEntity.clear();

        this->_gameStarted = false;
        this->_playerEntitiesCreated = false;
    }
}

void rserv::Lobby::enqueuePacket(std::pair<std::shared_ptr<net::INetworkEndpoint>,
    std::vector<uint8_t>> packet) {
    std::lock_guard<std::mutex> lock(_packetMutex);
    _incomingPackets.push(packet);
}

void rserv::Lobby::startNetworkThread() {
    _networkThread = std::thread([this]() { this->networkLoop(); });
}

void rserv::Lobby::startGameThread() {
    _gameThread = std::thread([this]() { this->gameLoop(); });
}

void rserv::Lobby::networkLoop() {
    while (_running && !Signal::stopFlag) {
        if (this->_running == false) {
            break;
        }

        {
            std::lock_guard<std::mutex> lock(_clientsMutex);
            if (this->_clients.empty()) {
                debug::Debug::printDebug(this->getIsDebug(),
                    "All clients disconnected. Stopping network loop.",
                    debug::debugType::NETWORK, debug::debugLevel::INFO);
                _running.store(false, std::memory_order_release);
                break;
            }
        }

        this->processIncomingPackets();

        bool shouldSendGameState = false;
        {
            std::lock_guard<std::mutex> lock(_clientsMutex);
            if (this->_clients.size() > 0) {
                auto gameStateNow = std::chrono::steady_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                    gameStateNow - this->_lastGameStateTime).count();
                if (elapsed >= 1000 / this->_tps) {
                    shouldSendGameState = true;
                    this->_lastGameStateTime = gameStateNow;
                }
            }
        }

        if (shouldSendGameState) {
            this->gameStatePacket();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        if (std::cin.eof()) {
            debug::Debug::printDebug(this->getIsDebug(),
                "EOF received (Ctrl+D pressed)",
                debug::debugType::NETWORK, debug::debugLevel::INFO);
            break;
        }
    }
}

void rserv::Lobby::gameLoop() {
    if (!this->_playerEntitiesCreated) {
        this->createPlayerEntities();
        this->_playerEntitiesCreated = true;
    }

    auto previousTime = std::chrono::high_resolution_clock::now();

    while (_running && !Signal::stopFlag) {
        if (this->_running == false) {
            break;
        }
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - previousTime).count();
        previousTime = currentTime;

        this->processLobbyEvents();

        if (this->_gsm) {
            this->_gsm->update(deltaTime);
        }

        this->_statusUpdateTimer += deltaTime;
        if (this->_statusUpdateTimer >= 2.0f) {
            this->serverStatusPacket();
            this->_statusUpdateTimer = 0.0f;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

bool rserv::Lobby::ackLeaveLobbyPacket(const net::INetworkEndpoint &endpoint,
    bool canDisconnect) {
    if (!this->_network) {
        debug::Debug::printDebug(this->getIsDebug(),
            "[SERVER] Warning: Network not initialized",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }
    if (!this->_packet) {
        debug::Debug::printDebug(this->getIsDebug(),
            "[SERVER] Warning: Packet manager not initialized",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }
    if (!this->_running.load(std::memory_order_acquire)) {
        debug::Debug::printDebug(this->getIsDebug(),
            "[SERVER] Warning: Lobby is no longer running, skipping ack packet",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }
    std::vector<uint64_t> payload;
    payload.push_back(static_cast<uint64_t>(canDisconnect ? 't' : 'f'));
    std::vector<uint8_t> packet = this->_packet->pack(constants::ID_SERVER,
        this->_sequenceNumber, constants::PACKET_ACK_LEAVE_LOBBY, payload);
    if (!this->_network->sendTo(endpoint, packet)) {
        debug::Debug::printDebug(this->getIsDebug(),
            "[SERVER] Failed to send LOBBY_CONNECT_VALUE response to client",
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return false;
    }
    this->_sequenceNumber++;
    debug::Debug::printDebug(this->getIsDebug(),
        "[SERVER] Sent ACK_LEAVE_LOBBY response to client: " +
        std::string(canDisconnect ? "success" : "failure"),
        debug::debugType::NETWORK, (canDisconnect ? debug::debugLevel::INFO :
            debug::debugLevel::WARNING));
    return true;
}
