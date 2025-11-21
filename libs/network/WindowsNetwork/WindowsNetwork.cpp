/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** WindowsNetwork
*/

#include "WindowsNetwork.hpp"
#include "../../../common/LoaderType.hpp"

namespace net {

WindowsNetwork::WindowsNetwork() {
}

WindowsNetwork::~WindowsNetwork() {
}

void WindowsNetwork::init() {
}

void WindowsNetwork::stop() {
}

void WindowsNetwork::sendData(const IPacket &data, size_t size) {
}

IPacket &WindowsNetwork::receiveData(const IBuffer &buffer, size_t size) const {
}

}  // namespace net

extern "C" {
    void *createNetworkInstance() {
        return new net::WindowsNetwork();
    }
    int getType() {
        return NETWORK_MODULE;
    }
}
