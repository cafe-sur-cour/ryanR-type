/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Header
*/

#ifndef ISERVER_HPP_
    #define ISERVER_HPP_

#include <memory>
#include <vector>
#include <asio.hpp>

#include "ServerConfig.hpp"
#include "../libs/Packet/IPacketManager.hpp"
#include "../libs/Network/INetwork.hpp"

namespace rserv {
class IServer {
public:
    virtual ~IServer() = default;

    virtual void init() = 0;
    virtual void start() = 0;
    virtual void stop() = 0;

    virtual void setConfig(std::shared_ptr<ServerConfig> config) = 0;
    virtual std::shared_ptr<ServerConfig> getConfig() const = 0;
    virtual uint16_t getPort() const = 0;
    virtual void setPort(uint16_t port) = 0;

    virtual int getState() const = 0;
    virtual void setState(int state) = 0;

    virtual int getFd() const = 0;
    virtual void setFd(int fd) = 0;
    virtual operator int() const noexcept = 0;

    virtual std::shared_ptr<net::INetwork> getNetwork() const = 0;
    virtual void setNetwork(std::shared_ptr<net::INetwork> network) = 0;

    virtual void onClientConnected(uint8_t idClient) = 0;
    virtual void onClientDisconnected(uint8_t idClient) = 0;
    virtual void onPacketReceived(uint8_t idClient, const pm::IPacketManager &packet) = 0;

    virtual bool processConnections(asio::ip::udp::endpoint id) = 0;
    virtual void processIncomingPackets() = 0;

    virtual void broadcastPacket() = 0;
    virtual void sendToClient(uint8_t idClient) = 0;
    virtual std::vector<uint8_t> getConnectedClients() const = 0;
    virtual size_t getClientCount() const = 0;
};
} // namespace rserv = r-type server
#endif /* !ISERVER_HPP_ */
