/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** WindowsNetwork
*/

#include "WindowsNetwork.hpp"
#include "../../../common/LoaderType.hpp"

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

IPacket &WindowsNetwork::receiveData(char *buffer, size_t size) const {
    IPacket *packet = nullptr;
    return *packet;
}

extern "C" {
    INetwork *createNetworkInstance() {
        return new WindowsNetwork();
    }
    int getType() {
        return NETWORK_MODULE;
    }
}
