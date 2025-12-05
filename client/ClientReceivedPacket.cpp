/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ClientPacket
*/

#include "ClientNetwork.hpp"
#include "../common/debug.hpp"


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
}

void ClientNetwork::handleMapSend() {
}

void ClientNetwork::handleEndMap() {
}

void ClientNetwork::handleEndGame() {
}

void ClientNetwork::handleCanStart() {
}
