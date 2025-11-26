/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** WindowsNetwork
*/

#include <vector>
#include <memory>
#include <stdexcept>

#include "WindowsNetwork.hpp"
#include "../../common/DLLoader/LoaderType.hpp"
#include "../../../common/DLLoader/LoaderType.hpp"

namespace net {

WindowsNetwork::WindowsNetwork() {
}

WindowsNetwork::~WindowsNetwork() {
}

void WindowsNetwork::init(int port) {
    (void)port;
}

void WindowsNetwork::stop() {
}


int WindowsNetwork::acceptConnection() {
    return -1;
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

void WindowsNetwork::sendTo(int connectionId, const IPacketManager &packet) {
    (void)connectionId;
    (void)packet;
}

void WindowsNetwork::broadcast(const IPacketManager &packet) {
    (void)packet;
}

bool WindowsNetwork::hasIncomingData() const {
    return false;
}

std::shared_ptr<IPacketManager> WindowsNetwork::receiveFrom(
    const int &connectionId) {
    (void)connectionId;
    return nullptr;
}

void WindowsNetwork::sendData(const IPacketManager &data, size_t size) {
    (void)data;
    (void)size;
}

IPacketManager &WindowsNetwork::receiveData(
    const IBuffer &buffer, size_t size) const {
    (void)buffer;
    (void)size;
    throw std::runtime_error("Not implemented");
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
        return NETWORK_MODULE;
    }
}
