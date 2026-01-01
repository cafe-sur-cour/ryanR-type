/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ClientPacket
*/

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include "ClientNetwork.hpp"
#include "../common/debug.hpp"
#include "../common/Parser/Parser.hpp"
#include "../common/ECS/entity/EntityCreationContext.hpp"
#include "../common/components/tags/LocalPlayerTag.hpp"
#include "DeathAnimationSpawner.hpp"
#include "gsm/states/scenes/InGame/InGameState.hpp"
#include "gsm/states/scenes/Results/ResultsState.hpp"
#include "./components/rendering/AnimationComponent.hpp"

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

void ClientNetwork::handleGameState() {
    auto payload = _packet->getPayload();
    if (payload.empty()) {
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] Game state packet is empty",
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
    size_t index = 0;

    if (index >= payload.size()) {
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] Invalid game state packet: missing entity ID",
            debug::debugType::NETWORK,
            debug::debugLevel::ERROR);
        return;
    }

    size_t serverEntityId = static_cast<size_t>(payload[index++]);
    auto it = _serverToLocalEntityMap.find(serverEntityId);
    if (it == _serverToLocalEntityMap.end()) {
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] Entity with server ID " +
            std::to_string(serverEntityId) + " not found in map",
            debug::debugType::NETWORK,
            debug::debugLevel::WARNING);
        return;
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
    this->_network->sendTo(_serverEndpoint, whoamiPacket);
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
        if (*it == static_cast<uint64_t>('\r')) {
            if (std::distance(it, payload.end()) >= 3 &&
                *(it + 1) == static_cast<uint64_t>('\n') &&
                *(it + 2) == static_cast<uint64_t>('\0')) {
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

    DeathAnimationSpawner::spawnDeathAnimation(_resourceManager, registry, entityId);

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
}
