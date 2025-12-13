/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ClientPacket
*/

#include <vector>
#include <string>
#include <memory>

#include "ClientNetwork.hpp"
#include "../common/debug.hpp"
#include "../common/Parser/Parser.hpp"
#include "../common/ECS/entity/EntityCreationContext.hpp"
#include "../common/components/permanent/NetworkIdComponent.hpp"
#include "../common/components/tags/LocalPlayerTag.hpp"
#include "gsm/states/scenes/InGame/InGameState.hpp"

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
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Received game state packet, starting parsing...",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);

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

    ecs::Entity entityId = static_cast<ecs::Entity>(payload[index++]);
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Processing entity ID: " + std::to_string(entityId),
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    while (index < payload.size()) {
        uint64_t componentType = payload[index++];
        auto it = _componentParsers.find(componentType);
        if (it != _componentParsers.end()) {
            index = (this->*(it->second))(payload, index, entityId);
        } else {
            debug::Debug::printDebug(this->_isDebug,
                "[CLIENT] Unknown component type: " + std::to_string(componentType),
                debug::debugType::NETWORK,
                debug::debugLevel::WARNING);
        }
    }
}

void ClientNetwork::handleMapSend() {
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Received map packet, starting parsing...",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    auto payload = _packet->getPayload();
    std::vector<uint8_t> payloadBytes;

    for (const auto &val : payload) {
        for (int j = 0; j < 8; ++j) {
            uint8_t byte = (val >> (j * 8)) & 0xFF;
            payloadBytes.push_back(byte);
        }
    }

    while (!payloadBytes.empty() && payloadBytes.back() == 0) {
        payloadBytes.pop_back();
    }
    auto parser = this->_resourceManager->get<Parser>();
    if (!parser) {
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] Parser not found in ResourceManager",
            debug::debugType::NETWORK,
            debug::debugLevel::ERROR);
        return;
    }
    try {
        parser->getMapParser()->setCreationContext(
            ecs::EntityCreationContext::forNetworkSync(0));
        parser->getMapParser()->parseMapFromPacket(payloadBytes);
    } catch (const std::exception& e) {
        debug::Debug::printDebug(this->_isDebug,
            std::string("[CLIENT] Error parsing map from packet: ") + e.what(),
            debug::debugType::NETWORK,
            debug::debugLevel::ERROR);
    }
}


void ClientNetwork::handleEndMap() {
}

void ClientNetwork::handleEndGame() {
}

void ClientNetwork::handleCanStart() {
    if (this->_ready) {
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] Can start packet already processed, ignoring duplicate",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return;
    }
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Received can start packet, starting parsing...",
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
    for (const auto &name : this->_clientNames) {
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] Player name: " + name,
            debug::debugType::NETWORK,
            debug::debugLevel::INFO);
    }

    if (this->_gsm) {
        this->_gsm->requestStateChange(
            std::make_shared<gsm::InGameState>(this->_gsm, this->_resourceManager)
        );
    }
}

void ClientNetwork::handleEntitySpawn() {
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Received entity spawn packet",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);

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

    ecs::Entity entity = _resourceManager->get<Parser>()->
        getPrefabManager()->createEntityFromPrefab(
        prefabName,
        _resourceManager->get<ecs::Registry>(),
        ecs::EntityCreationContext::forNetworkSync(0)
    );
    auto registry = _resourceManager->get<ecs::Registry>();
    registry->addComponent<ecs::NetworkIdComponent>(
        entity,
        std::make_shared<ecs::NetworkIdComponent>(clientId)
    );
}

void ClientNetwork::handleEntityDeath() {
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Received entity death packet",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);

    auto payload = _packet->getPayload();
    if (payload.size() < 1) {
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] Entity death packet is invalid",
            debug::debugType::NETWORK,
            debug::debugLevel::WARNING);
        return;
    }

    ecs::Entity entityId = static_cast<ecs::Entity>(payload.at(0));
    if (!this->_resourceManager->has<ecs::Registry>()) {
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] Registry not found in ResourceManager",
            debug::debugType::NETWORK,
            debug::debugLevel::ERROR);
        return;
    }
    auto registry = this->_resourceManager->get<ecs::Registry>();
    registry->destroyEntity(entityId);
}

void ClientNetwork::handleWhoAmI() {
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Received WHOAMI response",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);

    auto payload = _packet->getPayload();
    if (payload.size() < 1) {
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] WHOAMI packet is invalid",
            debug::debugType::NETWORK,
            debug::debugLevel::WARNING);
        return;
    }

    ecs::Entity entityId = static_cast<ecs::Entity>(payload.at(0));
    if (!this->_resourceManager->has<ecs::Registry>()) {
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] Registry not found in ResourceManager",
            debug::debugType::NETWORK,
            debug::debugLevel::ERROR);
        return;
    }
    auto registry = this->_resourceManager->get<ecs::Registry>();
    registry->registerComponent<ecs::LocalPlayerTag>();
    if (!registry->hasComponent<ecs::LocalPlayerTag>(entityId)) {
        registry->addComponent<ecs::LocalPlayerTag>
            (entityId, std::make_shared<ecs::LocalPlayerTag>());
    }
}
