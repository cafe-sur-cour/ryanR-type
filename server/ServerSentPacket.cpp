/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ServerSentPacket
*/

#include <vector>
#include <string>
#include <iostream>

#include "Server.hpp"
#include "Constants.hpp"
#include "../common/debug.hpp"
#include "../common/translationToECS.hpp"
#include "../common/ECS/entity/Entity.hpp"
#include "../common/ECS/entity/registry/Registry.hpp"
#include "../common/Parser/Parser.hpp"
#include "../common/components/permanent/NetworkIdComponent.hpp"

bool rserv::Server::connectionPacket(asio::ip::udp::endpoint endpoint) {
    std::vector<uint8_t> packet = this->_packet->pack(constants::ID_SERVER,
        this->_sequenceNumber, constants::PACKET_ACCEPT, std::vector<uint64_t>{
        static_cast<uint64_t>(this->_nextClientId)});

    if (!this->_network->sendTo(endpoint, packet)) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER NETWORK] Failed to send connection acceptance header to "
            + endpoint.address().to_string() + ":" + std::to_string(endpoint.port()),
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return false;
    }
    this->_sequenceNumber++;
    return true;
}

bool rserv::Server::gameStatePacket() {
    std::vector<uint64_t> payload;
    if (!this->_resourceManager->has<ecs::Registry>()) {
        return false;
    }
    auto registry = this->_resourceManager->get<ecs::Registry>();
    for (ecs::Entity i = 0; i < registry->getMaxEntityId(); i++) {
        payload.push_back(static_cast<uint64_t>(i));
        for (const auto& func : this->_convertFunctions) {
            std::vector<uint64_t> componentData = func(registry, i);
            payload.insert(payload.end(), componentData.begin(), componentData.end());
        }

        if (this->_network->broadcast(this->getConnectedClientEndpoints(),
            this->_packet->pack(constants::ID_SERVER, this->_sequenceNumber,
            constants::PACKET_GAME_STATE, payload)) == false) {
            debug::Debug::printDebug(this->_config->getIsDebug(),
                "[SERVER NETWORK] Failed to broadcast game state packet",
                debug::debugType::NETWORK, debug::debugLevel::ERROR);
            return false;
        }
        payload.clear();
    }
    return true;
}

bool rserv::Server::canStartPacket() {
    debug::Debug::printDebug(this->_config->getIsDebug(),
        "[SERVER] Checking canStart: clients=" +
        std::to_string(this->_clients.size()) + ", max=" +
        std::to_string(this->getConfig()->getNbClients()) + ", allReady=" +
        std::to_string(this->allClientsReady()),
        debug::debugType::NETWORK, debug::debugLevel::INFO);
    if (static_cast<int>(this->_clients.size()) ==
        this->getConfig()->getNbClients() && this->allClientsReady()) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] All clients are connected and ready, starting game",
            debug::debugType::NETWORK, debug::debugLevel::INFO);

        std::vector<uint64_t> payload;
        for (auto &client : this->_clients) {
            std::vector<uint64_t> name = this->_packet->formatString(std::get<2>(client));
            payload.insert(payload.end(), name.begin(), name.end());
        }
        std::vector<uint8_t> packetStart = this->_packet->pack(constants::ID_SERVER,
            this->_sequenceNumber, constants::PACKET_CAN_START, payload);

        if (!this->_network->broadcast(this->getConnectedClientEndpoints(), packetStart)) {
            debug::Debug::printDebug(this->_config->getIsDebug(),
                "[SERVER NETWORK] Failed to broadcast can start packet",
                debug::debugType::NETWORK, debug::debugLevel::ERROR);
            return false;
        }
        this->_sequenceNumber++;
        this->_gameStarted = true;
        std::string playerString = "player";
        auto prefabMgr = _resourceManager->get<EntityPrefabManager>();
        auto registry = _resourceManager->get<ecs::Registry>();
        size_t clientIndex = 0;
        for (const auto &client : this->_clients) {
            uint8_t clientId = std::get<0>(client);
            ecs::Entity playerEntity = prefabMgr->createEntityFromPrefab(
                playerString,
                registry
            );
            registry->registerComponent<ecs::NetworkIdComponent>();
            registry->addComponent<ecs::NetworkIdComponent>(
                playerEntity,
                std::make_shared<ecs::NetworkIdComponent>(clientId)
            );
            debug::Debug::printDebug(this->_config->getIsDebug(),
                "[SERVER] Created player entity " + std::to_string(playerEntity) +
                " for client " + std::to_string(static_cast<int>(clientId)),
                debug::debugType::NETWORK, debug::debugLevel::INFO);
            clientIndex++;
        }
        return true;
    }
    return false;
}

std::vector<uint64_t> rserv::Server::spawnPacket(size_t entity, const std::string prefabName) {
    std::vector<uint64_t> payload;

    payload.push_back(static_cast<uint64_t>(entity));
    for (const auto &c : prefabName) {
        payload.push_back(static_cast<uint64_t>(c));
    }
    payload.push_back(static_cast<uint64_t>('\r'));
    payload.push_back(static_cast<uint64_t>('\n'));
    payload.push_back(static_cast<uint64_t>('\0'));
    return payload;
}

std::vector<uint64_t> rserv::Server::deathPacket(size_t entity) {
    std::vector<uint64_t> payload;

    payload.push_back(static_cast<uint64_t>(entity));
    return payload;
}
