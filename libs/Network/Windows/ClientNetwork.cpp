/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** WindowsNetwork
*/

#include <iostream>
#include <stdexcept>
#include <string>
#include <memory>
#include <vector>
#include <utility>

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

void WindowsNetwork::disconnect() {
}

bool WindowsNetwork::isConnected() const {
    return _connected;
}

bool WindowsNetwork::sendTo(asio::ip::udp::endpoint id, std::vector<uint8_t> packet) {
    (void)id;
    (void)packet;
    return false;
}

bool WindowsNetwork::broadcast(std::vector<asio::ip::udp::endpoint> clients,
    std::vector<uint8_t> data) {
    (void)clients;
    (void)data;
    return false;
}

bool WindowsNetwork::hasIncomingData() const {
    return false;
}

std::vector<uint8_t> WindowsNetwork::receiveFrom(
    const uint8_t &connectionId) {
    (void)connectionId;
    return std::vector<uint8_t>();
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
