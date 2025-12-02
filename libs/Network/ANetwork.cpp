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

std::vector<int> net::ANetwork::getActiveConnections() const {
    std::vector<int> connections;
    for (const auto& [clientId, endpoint] : _clients) {
        connections.push_back(clientId);
    }
    return connections;
}

size_t net::ANetwork::getConnectionCount() const {
    return _clients.size();
}

void net::ANetwork::closeConnection(int connectionId) {
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

void net::ANetwork::setConnectionCallback(std::function<void(int)> onConnect) {
    _onConnectCallback = onConnect;
}

void net::ANetwork::setDisconnectionCallback(std::function<void(int)>
    onDisconnect) {
    _onDisconnectCallback = onDisconnect;
}

void net::ANetwork::setConnectionState(ConnectionState state) {
    this->_connectionState = state;
}

ConnectionState net::ANetwork::getConnectionState() const {
    return this->_connectionState;
}

}
