/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ClientPacket
*/

#include <vector>
#include <string>

#include "ClientNetwork.hpp"
#include "../common/debug.hpp"
#include "../common/Parser/Parser.hpp"

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
}
