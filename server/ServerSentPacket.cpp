/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ServerSentPacket
*/

#include <vector>
#include <iostream>

#include "Server.hpp"
#include "Constants.hpp"
#include "../common/debug.hpp"
#include "../common/translationToECS.hpp"
#include "../common/ECS/entity/Entity.hpp"
#include "../common/ECS/entity/registry/Registry.hpp"

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
        return true;
    }
    return false;
}
