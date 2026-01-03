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
#include <queue>
#include <asio.hpp>

#include "ServerConfig.hpp"
#include "../common/interfaces/IPacketManager.hpp"
#include "../common/interfaces/INetwork.hpp"
#include "../common/constants.hpp"

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

    virtual operator int() const noexcept = 0;

    virtual std::shared_ptr<net::INetwork> getNetwork() const = 0;
    virtual void setNetwork(std::shared_ptr<net::INetwork> network) = 0;

    virtual void onClientConnected(uint8_t idClient) = 0;
    virtual void onClientDisconnected(uint8_t idClient) = 0;
    virtual void onPacketReceived(uint8_t idClient, const pm::IPacketManager &packet) = 0;

    virtual void processIncomingPackets() = 0;
    virtual bool processConnections(std::pair<asio::ip::udp::endpoint, std::vector<uint8_t>> client) = 0;
    virtual bool processDisconnections(uint8_t idClient) = 0;
    virtual bool processEvents(uint8_t idClient) = 0;
    virtual bool processEndOfGame(uint8_t idClient) = 0;

    virtual std::vector<uint8_t> getConnectedClients() const = 0;
    virtual std::vector<asio::ip::udp::endpoint> getConnectedClientEndpoints() const = 0;
    virtual size_t getClientCount() const = 0;

    virtual std::shared_ptr<std::queue<std::tuple<uint8_t, constants::EventType, double>>> getEventQueue() = 0;
    virtual bool hasEvents() const = 0;
};
} // namespace rserv = r-type server
#endif /* !ISERVER_HPP_ */
