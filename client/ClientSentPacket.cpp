/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ClientSentPacket
*/

#include <cstring>
#include <iostream>
#include <vector>

#include "ClientNetwork.hpp"
#include "../common/Error/ClientNetworkError.hpp"
#include "../common/debug.hpp"


/* Packet Handling */
void ClientNetwork::connectionPacket() {
    if (!_network) {
        throw err::ClientNetworkError("[ClientNetwork] Network not initialized",
            err::ClientNetworkError::INTERNAL_ERROR);
    }
    std::vector<uint8_t> header = this->_packet->pack(this->_idClient,
        this->_sequenceNumber, 0x01);
    this->_network->sendTo(this->_serverEndpoint, header);
    /* Replace this with name */
    std::vector<uint64_t> payloadData = {0x01};
    for (size_t i = 0; i < this->_packet->formatString(this->_name).size(); i++)
        payloadData.push_back(this->_packet->formatString(this->_name)[i]);
    std::vector<uint8_t> payload = this->_packet->pack(payloadData);
    debug::Debug::printDebug(this->_isDebug,
        "[Client] Sending connection packet with name: " + this->_name,
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    this->_network->sendTo(this->_serverEndpoint, payload);
    this->_sequenceNumber++;
}


void ClientNetwork::eventPacket(const constants::EventType &eventType,
    double depth, double direction) {
    if (!_network) {
        throw err::ClientNetworkError("[ClientNetwork] Network not initialized",
            err::ClientNetworkError::INTERNAL_ERROR);
    }
    std::vector<uint8_t> header =
        this->_packet->pack(this->_idClient, this->_sequenceNumber, 0x02);
    this->_network->sendTo(this->_serverEndpoint, header);

    std::vector<uint64_t> payloadData;
    payloadData.push_back(0x02);
    payloadData.push_back(static_cast<uint64_t>(eventType));

    uint64_t depthBits;
    std::memcpy(&depthBits, &depth, sizeof(double));
    payloadData.push_back(depthBits);

    uint64_t dirBits;
    std::memcpy(&dirBits, &direction, sizeof(double));
    payloadData.push_back(dirBits);


    debug::Debug::printDebug(this->_isDebug,
        "[Client] Preparing to send event packet: EventType="
        + std::to_string(static_cast<int>(eventType))
        + ", Depth=" + std::to_string(depth)
        + ", Direction=" + std::to_string(direction),
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    if (payloadData.size() <= 3) {
        debug::Debug::printDebug(this->_isDebug,
            "[Client] Event packet payload size is invalid, aborting send.",
            debug::debugType::NETWORK,
            debug::debugLevel::ERROR);
        return;
    }
    std::vector<uint8_t> payload = this->_packet->pack(payloadData);
    this->_network->sendTo(this->_serverEndpoint, payload);
    this->_sequenceNumber++;
}


void ClientNetwork::disconnectionPacket() {
    if (!_network) {
        throw err::ClientNetworkError("[ClientNetwork] Network not initialized",
            err::ClientNetworkError::INTERNAL_ERROR);
    }
    if (this->_idClient == 0) {
        debug::Debug::printDebug(this->_isDebug,
            "[Client] Warning: Client ID is 0, cannot send disconnection packet",
            debug::debugType::NETWORK,
            debug::debugLevel::WARNING);
        return;
    }
    if (!this->_packet) {
        debug::Debug::printDebug(this->_isDebug,
            "[Client] Warning: Packet manager not initialized, cannot send disconnect packet",
            debug::debugType::NETWORK,
            debug::debugLevel::WARNING);
        return;
    }
    std::vector<uint8_t> header = this->_packet->pack(this->_idClient,
        this->_sequenceNumber, 0x03);
    this->_network->sendTo(this->_serverEndpoint, header);
    this->_sequenceNumber++;
}
