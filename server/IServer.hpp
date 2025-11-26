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

#include "ServerConfig.hpp"
#include "../libs/Packet/IPacket.hpp"
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
    virtual unsigned int getPort() const = 0;
    virtual void setPort(unsigned int port) = 0;

    virtual int getState() const = 0;
    virtual void setState(int state) = 0;

    virtual int getFd() const = 0;
    virtual void setFd(int fd) = 0;
    virtual operator int() const noexcept = 0;

    virtual std::shared_ptr<net::INetwork> getNetwork() const = 0;
    virtual void setNetwork(std::shared_ptr<net::INetwork> network) = 0;

    virtual void onClientConnected(int idClient) = 0;
    virtual void onClientDisconnected(int idClient) = 0;
    virtual void onPacketReceived(int idClient, const IPacket &packet) = 0;

    virtual void processConnections() = 0;
    virtual void processIncomingPackets() = 0;

    virtual void broadcastPacket() = 0;
    virtual void sendToClient(int idClient) = 0;
    virtual std::vector<int> getConnectedClients() const = 0;
    virtual size_t getClientCount() const = 0;
};
} // namespace rserv = r-type server
#endif /* !ISERVER_HPP_ */
