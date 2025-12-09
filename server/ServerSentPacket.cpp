/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ServerSentPacket
*/

#include <vector>
#include <iostream>

#include "Server.hpp"
#include "../common/debug.hpp"


bool rserv::Server::connectionPacket(asio::ip::udp::endpoint endpoint) {
    std::vector<uint8_t> packet = this->_packet->pack(0, this->_sequenceNumber,
        constants::PACKET_ACCEPT, std::vector<uint64_t>{
        static_cast<uint64_t>(this->_nextClientId)});

    if (!this->_network->sendTo(endpoint, packet)) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER NETWORK] Failed to send connection acceptance header to "
            + endpoint.address().to_string() + ":" + std::to_string(endpoint.port()),
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return false;
    }
    this->_sequenceNumber++;
    this->mapPacket(this->_currentMap, endpoint);
    return true;
}

bool rserv::Server::gameStatePacket() {
    return true;
}

bool rserv::Server::mapPacket(std::vector<uint64_t> mapData,
    const asio::ip::udp::endpoint &endpoint) {
        std::cout << "[SERVER] Sending map packet to "
            << endpoint.address().to_string() << ":"
            << endpoint.port() << std::endl;

    std::vector<uint8_t> packet = this->_packet->pack(0, this->_sequenceNumber,
        constants::PACKET_MAP, mapData);

    if (!this->_network->sendTo(endpoint, packet)) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER NETWORK] Failed to send map packet to "
            + endpoint.address().to_string() + ":" + std::to_string(endpoint.port()),
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return false;
    }
    this->_sequenceNumber++;
    return true;
}
