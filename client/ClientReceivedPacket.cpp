/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ClientPacket
*/

#include <vector>
#include <string>
#include <memory>
#include <utility>
#include <unordered_map>
#include "ClientNetwork.hpp"
#include "SettingsConfig.hpp"
#include "../common/debug.hpp"
#include "../common/Parser/Parser.hpp"
#include "../common/ECS/entity/EntityCreationContext.hpp"
#include "../common/components/tags/LocalPlayerTag.hpp"
#include "../common/components/tags/PlayerTag.hpp"
#include "../common/components/permanent/ScoreComponent.hpp"
#include "gsm/states/scenes/InGame/InGameState.hpp"
#include "gsm/states/scenes/Results/ResultsState.hpp"
#include "gsm/states/scenes/ForceLeave/ForceLeaveState.hpp"
#include "gsm/states/scenes/LevelComplete/LevelCompleteState.hpp"
#include "./components/rendering/AnimationComponent.hpp"
#include "../common/GameRules.hpp"

/* Packet Handlers */
void ClientNetwork::handleNoOp() {
    // No operation
}

void ClientNetwork::handleConnectionAcceptation() {
    auto payload = _packet->getPayload();
    if (payload.size() >= 1) {
        uint8_t id = static_cast<uint8_t>(payload.at(0));
        setIdClient(id);
        this->_isConnected = true;
        this->_network->setConnectionState(net::ConnectionState::CONNECTED);
        this->_packet->reset();
        this->_isConnected = true;
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] Connection accepted, assigned ID: " +
            std::to_string(static_cast<int>(id)),
            debug::debugType::NETWORK,
            debug::debugLevel::INFO);
    } else {
        this->_network->setConnectionState(net::ConnectionState::ERROR_STATE);
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] Connection acceptation failed: Invalid payload",
            debug::debugType::NETWORK,
            debug::debugLevel::ERROR);
    }
}

void ClientNetwork::handleBatchedGameState() {
    auto batchedPayloads = _packet->getBatchedPayloads();
    if (batchedPayloads.empty()) {
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] Batched game state packet is empty",
            debug::debugType::NETWORK,
            debug::debugLevel::WARNING);
        return;
    }

    if (!this->_resourceManager->has<ecs::Registry>()) {
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] Registry not found in ResourceManager",
            debug::debugType::NETWORK,
            debug::debugLevel::ERROR);
        return;
    }

    auto registry = this->_resourceManager->get<ecs::Registry>();

    for (const auto& payload : batchedPayloads) {
        if (payload.empty()) continue;

        size_t index = 0;
        size_t serverEntityId = static_cast<size_t>(payload[index++]);
        auto it = _serverToLocalEntityMap.find(serverEntityId);
        if (it == _serverToLocalEntityMap.end()) {
            debug::Debug::printDebug(this->_isDebug,
                "[CLIENT] Entity with server ID " +
                std::to_string(serverEntityId) + " not found in map",
                debug::debugType::NETWORK,
                debug::debugLevel::WARNING);
            continue;
        }
        ecs::Entity entityId = it->second;
        while (index < payload.size()) {
            uint64_t componentType = payload[index++];
            auto parserIt = _componentParsers.find(componentType);
            if (parserIt != _componentParsers.end()) {
                index = (this->*(parserIt->second))(payload, index, entityId);
            } else {
                debug::Debug::printDebug(this->_isDebug,
                    "[CLIENT] Unknown component type: " + std::to_string(componentType),
                    debug::debugType::NETWORK,
                    debug::debugLevel::WARNING);
            }
        }
    }

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Applied batched game state updates for " +
        std::to_string(batchedPayloads.size()) + " entities",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
}

void ClientNetwork::handleEndGame() {
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Received end game packet",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    auto payload = _packet->getPayload();
    bool isWin = false;
    if (!payload.empty()) {
        isWin = payload[0] != 0;
    }

    if (this->_gsm) {
        this->_gsm->requestStateChange(
            std::make_shared<gsm::ResultsState>
                (this->_gsm, this->_resourceManager, isWin)
        );
    }
}

void ClientNetwork::handleCanStart() {
    if (this->_ready) {
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] Can start packet already processed, ignoring duplicate",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return;
    }
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Received can start packet",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    std::vector<uint64_t> payload = _packet->getPayload();
    std::vector<std::string> names;

    for (size_t i = 0; i < payload.size(); i += 8) {
        std::string name;
        for (size_t j = 0; j < 8 && i + j < payload.size(); ++j) {
            char c = static_cast<char>(payload[i + j] & 0xFF);
            name += c;
        }
        name.erase(name.find_last_not_of(' ') + 1);
        names.push_back(name);
    }

    this->_clientNames = names;
    this->_ready = true;

    std::vector<uint64_t> whoamiPayload;
    std::vector<uint8_t> whoamiPacket = _packet->pack(_idClient, _sequenceNumber,
        constants::PACKET_WHOAMI, whoamiPayload);
    this->_network->sendTo(*_serverEndpoint, whoamiPacket);
    _sequenceNumber++;

    if (this->_gsm) {
        this->_gsm->requestStateChange(
            std::make_shared<gsm::InGameState>(this->_gsm, this->_resourceManager)
        );
    }
}

void ClientNetwork::handleEntitySpawn() {
    auto payload = _packet->getPayload();
    if (payload.empty()) {
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] Entity spawn packet is empty",
            debug::debugType::NETWORK,
            debug::debugLevel::WARNING);
        return;
    }

    size_t clientId = payload.at(0);
    std::string prefabName;
    for (auto it = payload.begin() + 1; it != payload.end(); ++it) {
        if (*it == static_cast<uint64_t>(constants::END_OFSTRING_ST)) {
            if (std::distance(it, payload.end()) >= 3 &&
                *(it + 1) == static_cast<uint64_t>(constants::END_OFSTRING_ND) &&
                *(it + 2) == static_cast<uint64_t>(constants::END_OFSTRING_TRD)) {
                break;
            }
        }
        prefabName += static_cast<char>(*it);
    }

    try {
        ecs::Entity newEntity = _resourceManager->get<Parser>()->
            getPrefabManager()->createEntityFromPrefab(
            prefabName,
            _resourceManager->get<ecs::Registry>(),
            ecs::EntityCreationContext::forLocalClient()
        );
        _serverToLocalEntityMap[clientId] = newEntity;
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] Created entity from prefab '" + prefabName +
                "' with serverId " + std::to_string(clientId) +
                " mapped to local " + std::to_string(newEntity),
            debug::debugType::NETWORK,
            debug::debugLevel::INFO);
    } catch (const std::exception& e) {
        debug::Debug::printDebug(this->_isDebug,
            std::string("[CLIENT] Error creating entity from prefab '")
                + prefabName + "': " + e.what(),
            debug::debugType::NETWORK,
            debug::debugLevel::ERROR);
        return;
    }
}

void ClientNetwork::handleEntityDeath() {
    auto payload = _packet->getPayload();
    if (payload.size() < 1) {
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] Entity death packet is invalid",
            debug::debugType::NETWORK,
            debug::debugLevel::WARNING);
        return;
    }

    size_t serverEntityId = static_cast<size_t>(payload.at(0));
    auto registry = this->_resourceManager->get<ecs::Registry>();
    auto it = _serverToLocalEntityMap.find(serverEntityId);
    if (it == _serverToLocalEntityMap.end()) {
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] Entity with server ID " + std::to_string(serverEntityId) +
                " not found for death",
            debug::debugType::NETWORK,
            debug::debugLevel::WARNING);
        return;
    }
    ecs::Entity entityId = it->second;


    registry->destroyEntity(entityId);
    _serverToLocalEntityMap.erase(serverEntityId);
}

void ClientNetwork::handleWhoAmI() {
    auto payload = _packet->getPayload();
    if (payload.size() < 1) {
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] WHOAMI packet is invalid",
            debug::debugType::NETWORK,
            debug::debugLevel::WARNING);
        return;
    }

    size_t serverPlayerId = static_cast<size_t>(payload.at(0));
    auto registry = this->_resourceManager->get<ecs::Registry>();
    auto it = _serverToLocalEntityMap.find(serverPlayerId);
    if (it == _serverToLocalEntityMap.end()) {
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] Player entity with server ID " + std::to_string(serverPlayerId) +
                " not found for WHOAMI",
            debug::debugType::NETWORK,
            debug::debugLevel::WARNING);
        return;
    }
    ecs::Entity entityId = it->second;
    registry->registerComponent<ecs::LocalPlayerTag>();
    if (!registry->hasComponent<ecs::LocalPlayerTag>(entityId)) {
        registry->addComponent<ecs::LocalPlayerTag>
            (entityId, std::make_shared<ecs::LocalPlayerTag>());
    }
    if (registry->hasComponent<ecs::LocalPlayerTag>(entityId) &&
        registry->hasComponent<ecs::AnimationComponent>(entityId)) {
        auto animComp = registry->getComponent<ecs::AnimationComponent>(entityId);
        std::unordered_map<std::string, std::shared_ptr<ecs::AnimationClip>>
            states = animComp->getStates();
        for (auto& kv : states) {
            const std::shared_ptr<ecs::AnimationClip>& originalClip = kv.second;
            if (!originalClip)
                continue;
            auto newClip = std::make_shared<ecs::AnimationClip>(*originalClip);
            newClip->startHeight = 0.0f;
            kv.second = newClip;
        }
        for (const auto& kv : states) {
            animComp->addState(kv.first, kv.second);
        }
    }
}

void ClientNetwork::handleServerStatus() {
    auto payload = _packet->getPayload();
    if (payload.size() < 4) {
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] SERVER_STATUS packet is invalid",
            debug::debugType::NETWORK,
            debug::debugLevel::WARNING);
        return;
    }
    this->_connectedClients = static_cast<size_t>(payload.at(0));
    this->_readyClients = static_cast<size_t>(payload.at(1));
    this->_clientId = static_cast<uint8_t>(payload.at(2));
    this->_clientReadyStatus = static_cast<bool>(payload.at(3));

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Server status: " + std::to_string(this->_connectedClients.load()) +
        " connected, " + std::to_string(this->_readyClients.load()) + " ready, client ID: " +
        std::to_string(this->_clientId.load()) + ", ready: " +
        std::to_string(this->_clientReadyStatus.load()),
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
}


void ClientNetwork::handleCode() {
    auto payload = _packet->getPayload();

    if (payload.size() < 8) {
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] REQUEST_LOBBY packet is invalid",
            debug::debugType::NETWORK,
            debug::debugLevel::WARNING);
        return;
    }
    std::string lobbyCode;
    for (size_t i = 0; i < 8; ++i) {
        char c = static_cast<char>(payload[i] & 0xFF);
        lobbyCode += c;
    }
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Received lobby code: " + lobbyCode,
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    _lobbyCode = lobbyCode;
    _isLobbyMaster = true;
    _isConnectedToLobby = true;
}

void ClientNetwork::handleLobbyConnectValue() {
    auto payload = _packet->getPayload();

    if (payload.size() < 1) {
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] LOBBY_CONNECT_VALUE packet is invalid",
            debug::debugType::NETWORK,
            debug::debugLevel::WARNING);
        return;
    }
    bool isSuccess = false;
    if (payload[0] == static_cast<uint64_t>('t')) {
        isSuccess = true;
    }

    bool isLeaving =
        this->_lobbyCode.find(constants::LOBBY_LEAVE_KEYWORD) != std::string::npos;

    if (isSuccess) {
        if (isLeaving) {
            debug::Debug::printDebug(this->_isDebug,
                "[CLIENT] Successfully left the lobby",
                debug::debugType::NETWORK,
                debug::debugLevel::INFO);
        } else {
            this->clearEntitiesAndMappings();
            debug::Debug::printDebug(this->_isDebug,
                "[CLIENT] Successfully connected to lobby",
                debug::debugType::NETWORK,
                debug::debugLevel::INFO);
        }
        _isConnectedToLobby = !isLeaving;
    } else {
        if (isLeaving) {
            debug::Debug::printDebug(this->_isDebug,
                "[CLIENT] Failed to leave the lobby",
                debug::debugType::NETWORK,
                debug::debugLevel::WARNING);
        } else {
            debug::Debug::printDebug(this->_isDebug,
                "[CLIENT] Failed to connect to lobby",
                debug::debugType::NETWORK,
                debug::debugLevel::WARNING);
        }
        if (!isLeaving) {
            this->_lobbyCode = "";
        }
        _isConnectedToLobby = false;
        _isLobbyMaster = false;
    }
}

void ClientNetwork::handleLevelComplete() {
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Received level complete packet",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);

    if (this->_gsm) {
        this->_gsm->requestStateChange(
            std::make_shared<gsm::LevelCompleteState>
                (this->_gsm, this->_resourceManager)
        );
    }
}

void ClientNetwork::handleNextLevel() {
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Received next level packet",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);

    _serverToLocalEntityMap.clear();

    if (this->_resourceManager->has<ecs::Registry>()) {
        auto registry = this->_resourceManager->get<ecs::Registry>();
        if (registry) {
            registry->clearAllEntities();
        }
    }

    if (this->_gsm) {
        this->_gsm->requestStateChange(
            std::make_shared<gsm::InGameState>
                (this->_gsm, this->_resourceManager)
        );
    }
}

void ClientNetwork::handleConnectUser() {
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Received connection user packet",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);

    auto payload = _packet->getPayload();
    std::string username;
    for (size_t i = 0; i < payload.size() && payload.at(i) !=
        constants::END_OFSTRING_TRD; ++i) {
        char c = static_cast<char>(payload.at(i) & 0xFF);
        username += c;
    }

    this->setName(username);
    this->_resourceManager->get<SettingsConfig>()->setUsername(username);
    this->_isConnected = true;

    if (this->_expectingLoginResponse) {
        if (auto gsm = this->_gsm) {
            gsm->requestStatePop();
        }
        this->_expectingLoginResponse = false;
    } else if (this->_expectingRegisterResponse) {
        if (auto gsm = this->_gsm) {
            gsm->requestStatePop();
        }
        this->_expectingRegisterResponse = false;
    }
}

void ClientNetwork::handleLeaderboard() {
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Received leaderboard packet",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);

    auto payload = _packet->getPayload();
    std::vector<std::pair<std::string, std::string>> leaderboardData;
    for (size_t i = 0; i < payload.size(); i += 16) {
        if (i + 16 > payload.size()) break;
        std::string username, scoreStr;
        size_t j = 0;
        for (; j < 8; ++j) {
            char c = static_cast<char>(payload.at(i + j) & 0xFF);
            if (c != constants::END_OFSTRING_TRD)
                username += c;
        }
        for (; j < 16; ++j) {
            char c = static_cast<char>(payload.at(i + j) & 0xFF);
            if (c != constants::END_OFSTRING_TRD)
                scoreStr += c;
        }
        if (!scoreStr.empty()) {
            leaderboardData.emplace_back(username, scoreStr);
        }
    }
    _leaderboardData = leaderboardData;
    _leaderboardDataUpdated = true;
}

void ClientNetwork::handleRegisterFail() {
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Received register fail packet",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);

    auto payload = _packet->getPayload();
    std::string errorMessage;
    for (size_t i = 0; i < payload.size() && payload.at(i) !=
        constants::END_OFSTRING_TRD; ++i) {
        char c = static_cast<char>(payload.at(i) & 0xFF);
        errorMessage += c;
    }

    if (errorMessage.empty()) {
        errorMessage = "Registration failed";
    }

    if (_resourceManager) {
        _resourceManager->add<std::string>(std::make_shared<std::string>(errorMessage));
    }

    this->_expectingRegisterResponse = false;
}

void ClientNetwork::handleProfile() {
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Received profile packet",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);

    auto payload = _packet->getPayload();
    std::vector<std::string> profileData;
    for (size_t i = 0; i < payload.size(); i += 8) {
        if (i + 8 > payload.size()) break;
        std::string field;
        for (size_t j = 0; j < 8; ++j) {
            char c = static_cast<char>(payload.at(i + j) & 0xFF);
            if (c != constants::END_OFSTRING_TRD)
                field += c;
        }
        profileData.push_back(field);
    }
    _profileData = profileData;
    _profileDataUpdated = true;
    this->_expectingProfileResponse = false;
}

void ClientNetwork::handleBroadcastedChat() {
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Received broadcasted chat packet",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);

    auto payload = _packet->getPayload();
    if (payload.size() < 8) {
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] BROADCASTED_CHAT packet is invalid",
            debug::debugType::NETWORK,
            debug::debugLevel::WARNING);
        return;
    }

    std::string username;
    for (size_t i = 0; i < 8; ++i) {
        char c = static_cast<char>(payload.at(i) & 0xFF);
        if (c == '\0')
            break;
        username += c;
    }

    std::string message;
    for (size_t i = 8; i < payload.size(); ++i) {
        char c = static_cast<char>(payload.at(i) & 0xFF);
        if (c == '\0')
            break;
        message += c;
    }

    if (this->_lastMessages.size() >= 10) {
        this->_lastMessages.erase(this->_lastMessages.begin());
    }
    this->_lastMessages.push_back(std::make_pair(username, message));
}

void ClientNetwork::handleGameRules() {
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Received game rules packet",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);

    auto payload = _packet->getPayload();
    if (payload.size() < 3) {
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] Game rules packet is invalid (size < 3)",
            debug::debugType::NETWORK,
            debug::debugLevel::WARNING);
        return;
    }

    GameRulesNS::Gamemode gamemode = static_cast<GameRulesNS::Gamemode>(payload.at(0));
    GameRulesNS::Difficulty difficulty = static_cast<GameRulesNS::Difficulty>(payload.at(1));
    bool crossfire = (payload.at(2) != 0);

    if (!this->_resourceManager->has<GameRules>()) {
        this->_resourceManager->add<GameRules>(std::make_shared<GameRules>());
    }

    auto gameRules = this->_resourceManager->get<GameRules>();
    gameRules->setGamemode(gamemode);
    gameRules->setDifficulty(difficulty);
    gameRules->setCrossfire(crossfire);
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Updated GameRules - gamemode: " +
            std::to_string(static_cast<int>(gamemode)) +
            ", difficulty: " + std::to_string(static_cast<int>(difficulty)) +
            ", crossfire: " + std::string(crossfire ? "ON" : "OFF"),
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
}

void ClientNetwork::handleForceLeave() {
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Received force leave packet",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);

    auto payload = _packet->getPayload();
    if (payload.size() < 1) {
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] Force leave packet is invalid",
            debug::debugType::NETWORK,
            debug::debugLevel::WARNING);
        return;
    }

    uint8_t leaveType = static_cast<uint8_t>(payload.at(0));
    constants::ForceLeaveType forceLeaveType =
        static_cast<constants::ForceLeaveType>(leaveType);

    this->_isConnectedToLobby = false;
    this->_isLobbyMaster = false;
    this->_lobbyCode = "";
    this->_ready = false;

    if (this->_gsm) {
        this->_gsm->requestStateChange(
            std::make_shared<gsm::ForceLeaveState>
                (this->_gsm, this->_resourceManager, forceLeaveType)
        );
    }
}

void ClientNetwork::handleAckLeaveLobby() {
    auto payload = _packet->getPayload();

    if (payload.size() < 1) {
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] LOBBY_CONNECT_VALUE packet is invalid",
            debug::debugType::NETWORK,
            debug::debugLevel::WARNING);
        return;
    }
    bool isSuccess = false;
    if (payload[0] == static_cast<uint64_t>('t')) {
        isSuccess = true;
    }
    if (isSuccess)
        this->_shouldDisconnect = true;
    else
        this->_shouldDisconnect = false;
}
