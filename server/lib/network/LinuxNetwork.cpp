/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LinuxNetwork
*/

#include "LinuxNetwork.hpp"
#include "../../../common/LoaderType.hpp"

LinuxNetwork::LinuxNetwork() {
}

LinuxNetwork::~LinuxNetwork() {
}


void LinuxNetwork::init() {
}

void LinuxNetwork::stop() {
}

void LinuxNetwork::sendData(const IPacket &data, size_t size) {
}

IPacket &LinuxNetwork::receiveData(char *buffer, size_t size) const {
    IPacket *packet = nullptr;
    return *packet;
}

extern "C" {
    INetwork *createNetworkInstance() {
        return new LinuxNetwork();
    }
    int getType() {
        return NETWORK_MODULE;
    }
}
