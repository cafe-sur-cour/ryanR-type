/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LinuxNetwork
*/

#include "LinuxNetwork.hpp"
#include "../../common/DLLoader/LoaderType.hpp"
#include "../Packet/Packet.hpp"

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
    (void)data;
    (void)size;
}

IPacket &LinuxNetwork::receiveData(const IBuffer &buffer, size_t size) const {
    (void)buffer;
    (void)size;
    static Packet tmpPacket(1);
    return tmpPacket;
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
