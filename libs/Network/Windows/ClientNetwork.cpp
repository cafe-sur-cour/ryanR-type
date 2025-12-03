/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** WindowsNetwork
*/

#include <vector>
#include <memory>
#include <stdexcept>
#include <utility>
#include <string>

#include "ClientNetwork.hpp"
#include "../../../common/DLLoader/LoaderType.hpp"

namespace net {

WindowsNetwork::WindowsNetwork() {
}

WindowsNetwork::~WindowsNetwork() {
}

void WindowsNetwork::init(uint16_t port, const std::string host) {
    (void)port;
    (void)host;
}

void WindowsNetwork::stop() {
}


uint8_t WindowsNetwork::acceptConnection(
    asio::ip::udp::endpoint id, std::shared_ptr<pm::IPacketManager> packetManager) {
    (void)id;
    (void)packetManager;
    return 0;
}

void WindowsNetwork::sendTo(asio::ip::udp::endpoint id, std::vector<uint8_t> packet) {
    (void)id;
    (void)packet;
}

void WindowsNetwork::broadcast(std::vector<uint8_t> data) {
    (void)packet;
}

bool WindowsNetwork::hasIncomingData() const {
    return false;
}

std::shared_ptr<pm::IPacketManager> WindowsNetwork::receiveFrom(
    const uint8_t &connectionId) {
    (void)connectionId;
    return nullptr;
}

std::pair<asio::ip::udp::endpoint, std::vector<uint8_t>> WindowsNetwork::receiveAny() {
    return std::make_pair(asio::ip::udp::endpoint(), std::vector<uint8_t>());
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
