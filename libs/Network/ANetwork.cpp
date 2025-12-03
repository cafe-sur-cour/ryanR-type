/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Abstract Network
*/

#include <iostream>
#include <vector>
#include "ANetwork.hpp"

namespace net {

std::vector<uint8_t> ANetwork::getActiveConnections() const {
    std::vector<uint8_t> connections;
    for (const auto& [clientId, endpoint] : _clients) {
        connections.push_back(clientId);
    }
    return connections;
}

size_t ANetwork::getConnectionCount() const {
    return _clients.size();
}

void ANetwork::closeConnection(uint8_t connectionId) {
    auto it = _clients.find(connectionId);
    if (it != _clients.end()) {
        std::cout << "[Network] Closing connection " <<
            connectionId << std::endl;
        _clients.erase(it);

        if (_onDisconnectCallback) {
            _onDisconnectCallback(connectionId);
        }
    }
}

void ANetwork::setConnectionCallback(
    std::function<void(int)> onConnect) {
    _onConnectCallback = onConnect;
}

void ANetwork::setDisconnectionCallback(std::function<void(int)>
    onDisconnect) {
    _onDisconnectCallback = onDisconnect;
}

void ANetwork::setConnectionState(ConnectionState state) {
    this->_connectionState = state;
}

ConnectionState ANetwork::getConnectionState() const {
    return this->_connectionState;
}

}
