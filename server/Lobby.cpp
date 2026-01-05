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
#include <string>
#include <vector>
#include <iostream>

#include "Lobby.hpp"
#include "Constants.hpp"
#include "../libs/Network/Unix/ServerNetwork.hpp"
#include "../common/components/tags/PlayerTag.hpp"
#include "../common/ECS/entity/Entity.hpp"
#include "../common/ECS/entity/registry/Registry.hpp"

#include "initResourcesManager/initResourcesManager.hpp"
#include "gsm/states/scenes/Boot/BootState.hpp"
#include "../common/debug.hpp"
#include "../../common/systems/systemManager/SystemManager.hpp"
#include "../../common/Prefab/entityPrefabManager/EntityPrefabManager.hpp"

rserv::Lobby::Lobby(std::shared_ptr<net::INetwork> network,
    std::vector<std::tuple<uint8_t, std::shared_ptr<net::INetworkEndpoint>,
    std::string>> lobbyPlayerInfo,
    std::string lobbyCode, bool debug) {
    this->_network = network;
    this->_clients = lobbyPlayerInfo;
    this->_lobbyCode = lobbyCode;
    this->_isDebug = debug;

    this->_clientsReady = std::map<uint8_t, bool>();
    this->_packet = nullptr;
    this->_sequenceNumber = 0;
    this->_resourceManager = nullptr;
    this->_gameStarted = false;
    this->_eventQueue = std::make_shared<std::queue<std::tuple<uint8_t,
        constants::EventType, double>>>();
    this->_lastGameStateTime = std::chrono::steady_clock::now();
    this->_statusUpdateTimer = 0.0f;
    this->_running = true;
    this->_gsm = nullptr;

    this->_convertFunctions = {
        std::bind(&rserv::Lobby::convertTagComponent, this,
            std::placeholders::_1, std::placeholders::_2),
        std::bind(&rserv::Lobby::convertTransformComponent, this,
            std::placeholders::_1, std::placeholders::_2),
        std::bind(&rserv::Lobby::convertSpeedComponent, this,
            std::placeholders::_1, std::placeholders::_2),
        std::bind(&rserv::Lobby::convertHealthComponent, this,
            std::placeholders::_1, std::placeholders::_2),
        std::bind(&rserv::Lobby::convertColliderComponent, this,
            std::placeholders::_1, std::placeholders::_2),
        std::bind(&rserv::Lobby::convertShootStatComponent, this,
            std::placeholders::_1, std::placeholders::_2),
        std::bind(&rserv::Lobby::convertScoreComponent, this,
            std::placeholders::_1, std::placeholders::_2),
        std::bind(&rserv::Lobby::convertDamageComponent, this,
            std::placeholders::_1, std::placeholders::_2),
        std::bind(&rserv::Lobby::convertLifetimeComponent, this,
            std::placeholders::_1, std::placeholders::_2),
        std::bind(&rserv::Lobby::convertVelocityComponent, this,
            std::placeholders::_1, std::placeholders::_2),
        std::bind(&rserv::Lobby::convertControllableTagComponent, this,
            std::placeholders::_1, std::placeholders::_2),
        std::bind(&rserv::Lobby::convertEnemyProjectileTagComponent, this,
            std::placeholders::_1, std::placeholders::_2),
        std::bind(&rserv::Lobby::convertGameZoneColliderTagComponent, this,
            std::placeholders::_1, std::placeholders::_2),
        std::bind(&rserv::Lobby::convertMobTagComponent, this,
            std::placeholders::_1, std::placeholders::_2),
        std::bind(&rserv::Lobby::convertObstacleTagComponent, this,
            std::placeholders::_1, std::placeholders::_2),
        std::bind(&rserv::Lobby::convertPlayerProjectileTagComponent, this,
            std::placeholders::_1, std::placeholders::_2),
        std::bind(&rserv::Lobby::convertShooterTagComponent, this,
            std::placeholders::_1, std::placeholders::_2),
        std::bind(&rserv::Lobby::convertProjectilePassThroughTagComponent, this,
            std::placeholders::_1, std::placeholders::_2),
        std::bind(&rserv::Lobby::convertProjectilePrefabComponent, this,
            std::placeholders::_1, std::placeholders::_2),
        std::bind(&rserv::Lobby::convertGameZoneComponent, this,
            std::placeholders::_1, std::placeholders::_2)
    };
}

rserv::Lobby::~Lobby() {
    this->stop();
}


/* Client Info Getter */
std::vector<uint8_t> rserv::Lobby::getConnectedClients() const {
    std::vector<uint8_t> clientIds;
    for (const auto &client : this->_clients) {
        clientIds.push_back(std::get<0>(client));
    }
    return clientIds;
}

std::vector<std::shared_ptr<net::INetworkEndpoint>>
    rserv::Lobby::getConnectedClientEndpoints() const {
    std::vector<std::shared_ptr<net::INetworkEndpoint>> endpoints;
    for (const auto &client : this->_clients) {
        endpoints.push_back(std::get<1>(client));
    }
    return endpoints;
}

size_t rserv::Lobby::getClientCount() const {
    return this->_clients.size();
}

std::shared_ptr<net::INetwork> rserv::Lobby::getNetwork() const {
    return this->_network;
}

std::string rserv::Lobby::getLobbyCode() const {
    return this->_lobbyCode;
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
    if (this->_packet->getType() == constants::PACKET_EVENT) {
        this->processEvents(this->_packet->getIdClient());
    } else if (this->_packet->getType() == constants::PACKET_WHOAMI) {
        this->processWhoAmI(this->_packet->getIdClient());
    } else {
        debug::Debug::printDebug(this->getIsDebug(),
            "[SERVER] Packet received of type "
            + std::to_string(static_cast<int>(this->_packet->getType()))
            + " from client: " + std::to_string(this->_packet->getIdClient()),
            debug::debugType::NETWORK, debug::debugLevel::INFO);
    }

    this->_packet->reset();
}

bool rserv::Lobby::processDisconnections(uint8_t idClient) {
    for (auto &client : this->_clients) {
        if (std::get<0>(client) == idClient) {
            this->_clients.erase(
                std::remove(this->_clients.begin(), this->_clients.end(), client),
                this->_clients.end());
            /* Send to server who disconnected ? */
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

bool rserv::Lobby::processEndOfGame(uint8_t idClient) {
    std::vector<uint8_t> packet = this->_packet->pack(constants::ID_SERVER,
        this->_sequenceNumber, constants::PACKET_END_GAME,
        std::vector<uint64_t>{static_cast<uint64_t>(idClient)});
    if (!this->_network->broadcast(this->getConnectedClientEndpoints(), packet)) {
        debug::Debug::printDebug(this->getIsDebug(),
            "[SERVER NETWORK] Failed to broadcast end of game packet",
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return false;
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
    auto playerView = registry->view<ecs::PlayerTag>();

    ecs::Entity playerEntity = 0;
    for (auto entityId : playerView) {
        if (entityId == idClient) {
            playerEntity = entityId;
            break;
        }
    }

    if (playerEntity == 0) {
        debug::Debug::printDebug(this->getIsDebug(),
            "[SERVER] No player entity found for client " + std::to_string(idClient),
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }

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
    };

    std::vector<EntityData> serializedEntities;
    serializedEntities.reserve(256);
    for (auto entityId : entityView) {
        EntityData entityData;
        entityData.entityId = static_cast<uint32_t>(entityId);
        entityData.componentData.reserve(64);
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
    for (auto& client : this->_clients) {
        uint8_t clientId = std::get<0>(client);

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

            std::vector<uint8_t> packet = this->_packet->pack(
                constants::ID_SERVER,
                this->_sequenceNumber++,
                constants::PACKET_GAME_STATE,
                payload
            );

            if (!this->_network->sendTo(*std::get<1>(client), packet)) {
                debug::Debug::printDebug(this->getIsDebug(),
                    "[SERVER NETWORK] Failed to send game state packet to client " +
                    std::to_string(static_cast<int>(clientId)),
                    debug::debugType::NETWORK, debug::debugLevel::ERROR);
                return false;
            }
        }
    }
    return true;
}

bool rserv::Lobby::endGamePacket(bool isWin) {
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
    size_t connectedClients = this->_clients.size();
    size_t readyClients = 0;

    for (const auto &client : this->_clients) {
        uint8_t clientId = std::get<0>(client);
        auto it = this->_clientsReady.find(clientId);
        if (it != this->_clientsReady.end() && it->second) {
            readyClients++;
        }
    }

    for (const auto &client : this->_clients) {
        uint8_t clientId = std::get<0>(client);
        bool isClientReady = false;
        auto it = this->_clientsReady.find(clientId);
        if (it != this->_clientsReady.end() && it->second) {
            isClientReady = true;
        }

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

        if (!this->_network->sendTo(*std::get<1>(client), packet)) {
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
        this->createPlayerEntities();
        this->_gsm = gsm;
        this->_gameStarted = true;
    } else {
        std::cerr << "Warning: Resource manager does not contain GSM" << std::endl;
    }
}

void rserv::Lobby::clearEntityDeltaCache(uint8_t clientId, uint32_t entityId) {
    this->_deltaTracker.clearEntityCache(clientId, entityId);
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
            "[LOBBY] Cannot create player entities: missing required" <<
            " components in resource manager",
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return;
    }

    if (this->_resourceManager->has<ecs::ServerInputProvider>()) {
        auto inputProvider = this->_resourceManager->get<ecs::ServerInputProvider>();
        if (inputProvider) {
            for (const auto &client : this->_clients) {
                uint8_t clientId = std::get<0>(client);
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
        << std::endl;
    for (const auto &client : this->_clients) {
        uint8_t clientId = std::get<0>(client);
        ecs::Entity playerEntity = prefabMgr->createEntityFromPrefab(
            playerString,
            registry,
            ecs::EntityCreationContext::forServer()
        );

        debug::Debug::printDebug(this->getIsDebug(),
            "[LOBBY] Created player entity " + std::to_string(playerEntity) +
            " for client " + std::to_string(static_cast<int>(clientId)),
            debug::debugType::NETWORK, debug::debugLevel::INFO);
    }
}

void rserv::Lobby::stop() {
    _running.store(false, std::memory_order_release);

    if (this->_networkThread.joinable()) {
        this->_networkThread.join();
    }
    if (this->_gameThread.joinable()) {
        this->_gameThread.join();
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
        this->processIncomingPackets();
        if (this->_clients.size() > 0) {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                now - this->_lastGameStateTime).count();
            if (elapsed >= constants::CD_TPS) {
                this->gameStatePacket();
                this->_lastGameStateTime = now;
            }
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
    auto previousTime = std::chrono::high_resolution_clock::now();

    while (_running && !Signal::stopFlag) {
        if (this->_running == false) {
            break;
        }
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - previousTime).count();
        previousTime = currentTime;

        this->processLobbyEvents();
        this->_gsm->update(deltaTime);
        this->_statusUpdateTimer += deltaTime;
        if (this->_statusUpdateTimer >= 2.0f) {
            this->serverStatusPacket();
            this->_statusUpdateTimer = 0.0f;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
