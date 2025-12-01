/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** WindowsNetwork
*/

#include <vector>
#include <memory>
#include <stdexcept>
#include <string>

#include "ClientNetwork.hpp"
#include "../../../common/DLLoader/LoaderType.hpp"

namespace net {

WindowsNetwork::WindowsNetwork() {
}

WindowsNetwork::~WindowsNetwork() {
}

void WindowsNetwork::init(uint32_t port, const std::string host) {
    (void)port;
    (void)host;
}

void WindowsNetwork::stop() {
}

uint8_t WindowsNetwork::acceptConnection() {
    return 1;
}

void WindowsNetwork::closeConnection(int connectionId) {
    (void)connectionId;
}

std::vector<int> WindowsNetwork::getActiveConnections() const {
    return std::vector<int>();
}

size_t WindowsNetwork::getConnectionCount() const {
    return 0;
}

void WindowsNetwork::sendTo(int connectionId, const pm::IPacketManager &packet) {
    (void)connectionId;
    (void)packet;
}

void WindowsNetwork::broadcast(const pm::IPacketManager &packet) {
    (void)packet;
}

bool WindowsNetwork::hasIncomingData() const {
    return false;
}

std::shared_ptr<pm::IPacketManager> WindowsNetwork::receiveFrom(
    const int &connectionId) {
    (void)connectionId;
    return nullptr;
}

void WindowsNetwork::setConnectionCallback(std::function<void(int)> onConnect) {
    (void)onConnect;
}

void WindowsNetwork::setDisconnectionCallback(std::function<void(int)>
    onDisconnect) {
    (void)onDisconnect;
}

}
extern "C" {
    void *createNetworkInstance() {
        return new net::WindowsNetwork();
    }
    int getType() {
        return NETWORK_CLIENT_MODULE;
    }
}
