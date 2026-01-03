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
#include "../common/constants.hpp"

/* Packet Handling */
void ClientNetwork::connectionPacket() {
    if (!_network) {
        throw err::ClientNetworkError("[ClientNetwork] Network not initialized",
            err::ClientNetworkError::INTERNAL_ERROR);
    }

    std::vector<uint64_t> payload = this->_packet->formatString(this->_name);
    std::vector<uint8_t> packet = this->_packet->pack(this->_idClient,
        this->_sequenceNumber, constants::PACKET_CONNECTION, payload);

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Sending connection packet with name: " + this->_name,
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);

    this->_network->sendTo(*this->_serverEndpoint, packet);
    this->_sequenceNumber++;
}


void ClientNetwork::eventPacket(const constants::EventType &eventType,
    double depth) {
    if (!_network) {
        throw err::ClientNetworkError("[ClientNetwork] Network not initialized",
            err::ClientNetworkError::INTERNAL_ERROR);
    }

    uint64_t depthBits;
    std::memcpy(&depthBits, &depth, sizeof(double));

    std::vector<uint64_t> payload;
    payload.push_back(static_cast<uint64_t>(eventType));
    payload.push_back(depthBits);

    std::vector<uint8_t> packet =
        this->_packet->pack(this->_idClient, this->_sequenceNumber,
        constants::PACKET_EVENT, payload);

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Preparing to send event packet: EventType="
        + std::to_string(static_cast<int>(eventType))
        + ", Depth=" + std::to_string(depth),
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    this->_network->sendTo(*this->_serverEndpoint, packet);
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
    std::vector<uint64_t> payload = {};
    payload.push_back(static_cast<uint64_t>(this->_idClient));

    std::vector<uint8_t> header = this->_packet->pack(this->_idClient,
        this->_sequenceNumber, constants::PACKET_DISC, payload);

    this->_network->sendTo(*this->_serverEndpoint, header);
    this->_sequenceNumber++;
}

void ClientNetwork::sendReady() {
    if (!_network) {
        throw err::ClientNetworkError("[ClientNetwork] Network not initialized",
            err::ClientNetworkError::INTERNAL_ERROR);
    }
    if (this->_idClient == 0) {
        debug::Debug::printDebug(this->_isDebug,
            "[Client] Warning: Client ID is 0, cannot send ready packet",
            debug::debugType::NETWORK,
            debug::debugLevel::WARNING);
        return;
    }
    std::vector<uint64_t> payload = {};
    std::vector<uint8_t> packet = this->_packet->pack(this->_idClient,
        this->_sequenceNumber, constants::PACKET_CLIENT_READY, payload);

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Sending ready packet",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);

    this->_network->sendTo(*this->_serverEndpoint, packet);
    this->_sequenceNumber++;
}

void ClientNetwork::sendWhoAmI() {
    if (!_network) {
        throw err::ClientNetworkError("[ClientNetwork] Network not initialized",
            err::ClientNetworkError::INTERNAL_ERROR);
    }
    if (this->_idClient == 0) {
        debug::Debug::printDebug(this->_isDebug,
            "[Client] Warning: Client ID is 0, cannot send WHOAMI packet",
            debug::debugType::NETWORK,
            debug::debugLevel::WARNING);
        return;
    }
    std::vector<uint64_t> payload = {};
    std::vector<uint8_t> packet = this->_packet->pack(this->_idClient,
        this->_sequenceNumber, constants::PACKET_WHOAMI, payload);

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Sending WHOAMI packet",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);

    this->_network->sendTo(*this->_serverEndpoint, packet);
    this->_sequenceNumber++;
}

void ClientNetwork::requestCode() {
    if (!_network) {
        throw err::ClientNetworkError("[ClientNetwork] Network not initialized",
            err::ClientNetworkError::INTERNAL_ERROR);
    }
    if (this->_idClient == 0) {
        debug::Debug::printDebug(this->_isDebug,
            "[Client] Warning: Client ID is 0, cannot send request code packet",
            debug::debugType::NETWORK,
            debug::debugLevel::WARNING);
        return;
    }
    std::vector<uint64_t> payload = {};
    std::vector<uint8_t> packet = this->_packet->pack(this->_idClient,
        this->_sequenceNumber, constants::PACKET_REQUEST_LOBBY, payload);

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Sending request code packet",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    std::cout << std::endl;
    this->_network->sendTo(*this->_serverEndpoint, packet);
    this->_sequenceNumber++;
}
