/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** UnixNetwork
*/

#include "UnixNetwork.hpp"
#include "../../common/DLLoader/LoaderType.hpp"

namespace net {

UnixNetwork::UnixNetwork() {
}

UnixNetwork::~UnixNetwork() {
}


void UnixNetwork::init() {
}

void UnixNetwork::stop() {
}

void UnixNetwork::sendData(const IPacket &data, size_t size) {
}

IPacket &UnixNetwork::receiveData(const IBuffer &buffer, size_t size) const {
}

}  // namespace net

extern "C" {
    void *createNetworkInstance() {
        return new net::UnixNetwork();
    }
    int getType() {
        return NETWORK_MODULE;
    }
}
