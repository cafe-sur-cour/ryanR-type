/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ServerReceivePacket
*/

#include <vector>
#include <iostream>
#include <string>

#include "Server.hpp"
#include "../common/debug.hpp"


bool rserv::Server::processConnections(std::pair<asio::ip::udp::endpoint,
    std::vector<uint8_t>> client) {
    if (!_network) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Warning: Network not initialized",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }
    std::string name = "";
    if (client.second.size() > HEADER_SIZE)
        name = std::string(client.second.begin() + HEADER_SIZE, client.second.end());

    if (this->_nextClientId > this->getConfig()->getNbClients()) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Warning: Maximum clients reached",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }
    this->connectionPacket(client.first);
    this->_clients.push_back(std::make_tuple(this->_nextClientId, client.first, name));
    this->_clientsReady[this->_nextClientId] = false;
    debug::Debug::printDebug(this->_config->getIsDebug(), "[SERVER] Set client " +
        std::to_string(this->_nextClientId) + " ready to false",
        debug::debugType::NETWORK, debug::debugLevel::INFO);
    this->canStartPacket();
    this->_nextClientId++;
    return true;
}


bool rserv::Server::processDisconnections(uint8_t idClient) {
    for (auto &client : this->_clients) {
        if (std::get<0>(client) == idClient) {
            this->_clients.erase(
                std::remove(this->_clients.begin(), this->_clients.end(), client),
                this->_clients.end());
            this->_nextClientId--;
            debug::Debug::printDebug(this->_config->getIsDebug(),
                "Client " + std::to_string(idClient)
                + " disconnected and removed from the lobby",
                debug::debugType::NETWORK, debug::debugLevel::INFO);
            return true;
        }
    }
    return false;
}

bool rserv::Server::processEndOfGame(uint8_t idClient) {
    std::vector<uint8_t> packet = this->_packet->pack(0, this->_sequenceNumber,
        constants::PACKET_END_GAME, std::vector<uint64_t>{static_cast<uint64_t>(idClient)});
    if (!this->_network->broadcast(this->getConnectedClientEndpoints(), packet)) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER NETWORK] Failed to broadcast end of game packet",
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return false;
    }
    return true;
}


bool rserv::Server::processEvents(uint8_t idClient) {
    debug::Debug::printDebug(this->_config->getIsDebug(),
        "[SERVER] Processing event packet from client: "
        + std::to_string(idClient),
        debug::debugType::NETWORK, debug::debugLevel::INFO);
    constants::EventType eventType =
        static_cast<constants::EventType>(this->_packet->getPayload().at(0));

    uint64_t param1Bits = this->_packet->getPayload().at(1);
    double param1;
    std::memcpy(&param1, &param1Bits, sizeof(double));

    this->_eventQueue->push(std::tuple(idClient, eventType, param1));
    return true;
}
