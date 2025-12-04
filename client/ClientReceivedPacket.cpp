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
        uint8_t id = static_cast<uint8_t>(payload[0]);
        setIdClient(id);
        this->_isConnected = true;
        this->_network->setConnectionState(net::ConnectionState::CONNECTED);
        this->_packet->reset();
        debug::Debug::printDebug(this->_isDebug,
            "[Client] Connection accepted, assigned ID: " +
            std::to_string(static_cast<int>(id)),
            debug::debugType::NETWORK,
            debug::debugLevel::INFO);
    } else {
        this->_network->setConnectionState(net::ConnectionState::ERROR_STATE);
        debug::Debug::printDebug(this->_isDebug,
            "[Client] Connection acceptation failed: Invalid payload",
            debug::debugType::NETWORK,
            debug::debugLevel::ERROR);
    }
}

void ClientNetwork::handleGameState() {
    auto payload = _packet->getPayload();
    if (payload.size() >= 2) {
        uint8_t playerId = static_cast<uint8_t>(payload[0]);
        debug::Debug::printDebug(this->_isDebug,
            "[Client] Game state update for player " +
            std::to_string(static_cast<int>(playerId)),
            debug::debugType::NETWORK,
            debug::debugLevel::INFO);
    }
}

void ClientNetwork::handleMapSend() {
    auto payload = _packet->getPayload();
    if (payload.size() >= 2) {
        uint8_t playerId = static_cast<uint8_t>(payload[0]);
        debug::Debug::printDebug(this->_isDebug,
            "[Client] Map data received for player " +
            std::to_string(static_cast<int>(playerId)),
            debug::debugType::NETWORK,
            debug::debugLevel::INFO);
    }
}

void ClientNetwork::handleEndMap() {
    auto payload = _packet->getPayload();
    if (payload.size() >= 1) {
        uint8_t playerId = static_cast<uint8_t>(payload[0]);
        debug::Debug::printDebug(this->_isDebug,
            "[Client] End of map for player " + std::to_string(static_cast<int>(playerId)),
            debug::debugType::NETWORK,
            debug::debugLevel::INFO);
    }
}

void ClientNetwork::handleEndGame() {
    auto payload = _packet->getPayload();
    if (payload.size() >= 2) {
        uint8_t playerId = static_cast<uint8_t>(payload[0]);
        uint8_t winnerId = static_cast<uint8_t>(payload[1]);
        debug::Debug::printDebug(this->_isDebug,
            "[Client] Game ended, winner: " + std::to_string(static_cast<int>(winnerId)),
            debug::debugType::NETWORK,
            debug::debugLevel::INFO);
        (void)playerId;
    }
}

void ClientNetwork::handleCanStart() {
    auto payload = _packet->getPayload();
    if (payload.size() >= 1) {
        uint8_t playerId = static_cast<uint8_t>(payload[0]);
        debug::Debug::printDebug(this->_isDebug,
            "[Client] Can start game for player " + std::to_string(static_cast<int>(playerId)),
            debug::debugType::NETWORK,
            debug::debugLevel::INFO);
        // Enable game start
    }
}
