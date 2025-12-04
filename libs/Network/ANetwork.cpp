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
