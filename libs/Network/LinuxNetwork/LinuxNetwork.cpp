/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LinuxNetwork
*/

#include "LinuxNetwork.hpp"
#include "../../DLLoader/LoaderType.hpp"

namespace net {

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

IPacket &LinuxNetwork::receiveData(const IBuffer &buffer, size_t size) const {
}

}  // namespace net

extern "C" {
    void *createNetworkInstance() {
        return new net::LinuxNetwork();
    }
    int getType() {
        return NETWORK_MODULE;
    }
}
