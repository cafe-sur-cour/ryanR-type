/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Server Libraries Loading
*/

#include <memory>
#include "Server.hpp"
#include "../common/Error/ServerErrror.hpp"

void rserv::Server::loadNetworkLibrary() {
    if (!_networloader.Open(pathLoad "/" networkLib sharedLibExt)) {
        throw err::ServerError(
            "[Server] Cannot open network library",
            err::ServerError::LIBRARY_LOAD_FAILED);
    }
    if (!_networloader.getHandler()) {
        throw err::ServerError(
            "[Server] Cannot get network library handler",
            err::ServerError::LIBRARY_LOAD_FAILED);
    }
    createNetworkLib_t createNetwork = _networloader.getSymbol
        ("createNetworkInstance");
    if (!createNetwork) {
        throw err::ServerError(
            "[Server] Cannot get createNetworkInstance symbol",
            err::ServerError::LIBRARY_LOAD_FAILED);
    }
    _network = std::shared_ptr<net::INetwork>
        (reinterpret_cast<net::INetwork *>(createNetwork()));
    if (!_network) {
        throw err::ServerError(
            "[Server] Creating network instance failed",
            err::ServerError::LIBRARY_LOAD_FAILED);
    }
}

void rserv::Server::loadBufferLibrary() {
    if (!_bufferloader.Open(pathLoad "/" bufferLib sharedLibExt)) {
        throw err::ServerError(
            "[Server] Cannot open buffer library",
            err::ServerError::LIBRARY_LOAD_FAILED);
    }
    if (!_bufferloader.getHandler()) {
        throw err::ServerError(
            "[Server] Cannot get buffer library handler",
            err::ServerError::LIBRARY_LOAD_FAILED);
    }
    createBuffer_t createBuffer = _bufferloader.getSymbol
        ("createBufferInstance");
    if (!createBuffer) {
        throw err::ServerError(
            "[Server] Cannot get createBufferInstance symbol",
            err::ServerError::LIBRARY_LOAD_FAILED);
    }
    _buffer = std::shared_ptr<IBuffer>
        (reinterpret_cast<IBuffer *>(createBuffer()));
    if (!_buffer) {
        throw err::ServerError(
            "[Server] Creating buffer instance failed",
            err::ServerError::LIBRARY_LOAD_FAILED);
    }
}

void rserv::Server::loadPacketLibrary() {
    if (!_packetloader.Open(pathLoad "/" packetLib sharedLibExt)) {
        throw err::ServerError(
            "[Server] Cannot open packet library",
            err::ServerError::LIBRARY_LOAD_FAILED);
    }
    if (!_packetloader.getHandler()) {
        throw err::ServerError(
            "[Server] Cannot get packet library handler",
            err::ServerError::LIBRARY_LOAD_FAILED);
    }
    createPacket_t createPacket = _packetloader.getSymbol
        ("createPacketInstance");
    if (!createPacket) {
        throw err::ServerError(
            "[Server] Cannot get createPacketInstance symbol",
            err::ServerError::LIBRARY_LOAD_FAILED);
    }
    _packet = std::shared_ptr<pm::IPacketManager>
        (reinterpret_cast<pm::IPacketManager *>(createPacket()));
    if (!_packet) {
        throw err::ServerError(
            "[Server] Creating packet instance failed",
            err::ServerError::LIBRARY_LOAD_FAILED);
    }
}
