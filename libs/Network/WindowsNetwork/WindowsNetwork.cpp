/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** WindowsNetwork
*/

#include <vector>
#include <memory>

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
}

void WindowsNetwork::closeConnection(int connectionId) {
    (void)connectionId;
}

std::vector<int> WindowsNetwork::getActiveConnections() const {
}

size_t WindowsNetwork::getConnectionCount() const {
}

void WindowsNetwork::sendTo(int connectionId, const IPacket &packet) {
    (void)connectionId;
    (void)packet;
}

void WindowsNetwork::broadcast(const IPacket &packet) {
    (void)packet;
}

bool WindowsNetwork::hasIncomingData() const {
}

std::shared_ptr<IPacket> WindowsNetwork::receiveFrom(const int &connectionId) {
    (void)connectionId;
}

void WindowsNetwork::sendData(const IPacket &data, size_t size) {
    (void)data;
    (void)size;
}

IPacket &WindowsNetwork::receiveData(const IBuffer &buffer, size_t size) const {
    (void)buffer;
    (void)size;
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
