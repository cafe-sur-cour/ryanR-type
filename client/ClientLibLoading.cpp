/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ClientLibLoading
*/

#include <memory>

#include "ClientNetwork.hpp"
#include "../common/Error/ClientNetworkError.hpp"


void ClientNetwork::loadNetworkLibrary() {
    if (!_networloader.Open(pathLoad "/" networkClientLib sharedLibExt)) {
        throw err::ClientNetworkError("[ClientNetwork] Loading network lib failed",
            err::ClientNetworkError::LIBRARY_LOAD_FAILED);
    }
    if (!_networloader.getHandler()) {
        throw err::ClientNetworkError("[ClientNetwork] Loading network lib failed",
            err::ClientNetworkError::LIBRARY_LOAD_FAILED);
    }
    createNetworkLib_t createNetwork = _networloader.getSymbol
        ("createNetworkInstance");
    if (!createNetwork) {
        throw err::ClientNetworkError("[ClientNetwork] Loading network lib failed",
            err::ClientNetworkError::LIBRARY_LOAD_FAILED);
    }
    _network = std::shared_ptr<net::INetwork>
        (reinterpret_cast<net::INetwork *>(createNetwork()));
    if (!_network) {
        throw err::ClientNetworkError("[ClientNetwork] Loading network lib failed",
            err::ClientNetworkError::LIBRARY_LOAD_FAILED);
    }
}

void ClientNetwork::loadBufferLibrary() {
    if (!_bufferloader.Open(pathLoad "/" bufferLib sharedLibExt)) {
        throw err::ClientNetworkError("[ClientNetwork] Loading buffer lib failed",
            err::ClientNetworkError::LIBRARY_LOAD_FAILED);
    }
    if (!_bufferloader.getHandler()) {
        throw err::ClientNetworkError("[ClientNetwork] Loading buffer lib failed",
            err::ClientNetworkError::LIBRARY_LOAD_FAILED);
    }
    createBuffer_t createBuffer = _bufferloader.getSymbol
        ("createBufferInstance");
    if (!createBuffer) {
        throw err::ClientNetworkError("[ClientNetwork] Loading buffer lib failed",
            err::ClientNetworkError::LIBRARY_LOAD_FAILED);
    }
    this->_receptionBuffer = std::shared_ptr<IBuffer>
        (reinterpret_cast<IBuffer *>(createBuffer()));
    if (!this->_receptionBuffer) {
        throw err::ClientNetworkError("[ClientNetwork] Loading buffer lib failed",
            err::ClientNetworkError::LIBRARY_LOAD_FAILED);
    }
    this->_sendBuffer = std::shared_ptr<IBuffer>
        (reinterpret_cast<IBuffer *>(createBuffer()));
    if (!this->_sendBuffer) {
        throw err::ClientNetworkError("[ClientNetwork] Loading buffer lib failed",
            err::ClientNetworkError::LIBRARY_LOAD_FAILED);
    }
}

void ClientNetwork::loadPacketLibrary() {
    if (!_packetloader.Open(pathLoad "/" packetLib sharedLibExt)) {
        throw err::ClientNetworkError("[ClientNetwork] Loading packet lib failed",
            err::ClientNetworkError::LIBRARY_LOAD_FAILED);
    }
    if (!_packetloader.getHandler()) {
        throw err::ClientNetworkError("[ClientNetwork] Loading packet lib failed",
            err::ClientNetworkError::LIBRARY_LOAD_FAILED);
    }
    createPacket_t createPacket = _packetloader.getSymbol
        ("createPacketInstance");
    if (!createPacket) {
        throw err::ClientNetworkError("[ClientNetwork] Loading packet lib failed",
            err::ClientNetworkError::LIBRARY_LOAD_FAILED);
    }
    _packet = std::shared_ptr<pm::IPacketManager>
        (reinterpret_cast<pm::IPacketManager *>(createPacket()));
    if (!_packet) {
        throw err::ClientNetworkError("[ClientNetwork] Loading packet lib failed",
            err::ClientNetworkError::LIBRARY_LOAD_FAILED);
    }
}
