/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** INetwork
*/

#ifndef INETWORK_HPP_
#define INETWORK_HPP_

#include <vector>
#include <functional>
#include <asio.hpp>
#include "../Packet/IPacketManager.hpp"
#include "../Buffer/IBuffer.hpp"

namespace net {

class INetwork {
    public:

        virtual ~INetwork() = default;

        virtual void init(uint16_t port, const std::string host) = 0;
        virtual void stop() = 0;

        virtual uint8_t acceptConnection(asio::ip::udp::endpoint id, std::shared_ptr<pm::IPacketManager> packetManager) = 0;
        virtual void closeConnection(uint8_t connectionId) = 0;
        virtual std::vector<uint8_t> getActiveConnections() const = 0;
        virtual size_t getConnectionCount() const = 0;

        virtual void sendTo(asio::ip::udp::endpoint id, std::vector<uint8_t> packet) = 0;
        virtual void broadcast(const pm::IPacketManager &packet) = 0;
        virtual bool hasIncomingData() const = 0;
        virtual std::shared_ptr<pm::IPacketManager> receiveFrom(const uint8_t &connectionId) = 0;
        virtual std::pair<asio::ip::udp::endpoint, std::vector<uint8_t>> receiveAny() = 0;

        virtual void setConnectionCallback(std::function<void(int)> onConnect) = 0;
        virtual void setDisconnectionCallback(std::function<void(int)> onDisconnect) = 0;

    protected:
    private:
};

} // namespace net

#endif /* !INETWORK_HPP_ */
