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
#include <memory>
#include "INetworkEndpoint.hpp"
#include "IPacketManager.hpp"
#include "IBuffer.hpp"

namespace net {

enum class ConnectionState {
    DISCONNECTED,
    CONNECTING,
    CONNECTED,
    RECONNECTING,
    ERROR_STATE
};


class INetwork {
    public:

        virtual ~INetwork() = default;

        virtual void init(uint16_t port, const std::string host) = 0;
        virtual void stop() = 0;

        virtual bool sendTo(const INetworkEndpoint& endpoint, std::vector<uint8_t> packet) = 0;
        virtual bool broadcast(const std::vector<std::shared_ptr<INetworkEndpoint>>& endpoints, const std::vector<uint8_t>& data) = 0;
        virtual bool hasIncomingData() const = 0;
        virtual std::vector<uint8_t> receiveFrom(const uint8_t &connectionId) = 0;
        virtual std::pair<std::shared_ptr<INetworkEndpoint>, std::vector<uint8_t>> receiveAny() = 0;

        virtual void setConnectionCallback(std::function<void(int)> onConnect) = 0;
        virtual void setDisconnectionCallback(std::function<void(int)> onDisconnect) = 0;
        virtual ConnectionState getConnectionState() const = 0;
        virtual void setConnectionState(ConnectionState state) = 0;
};

} // namespace net

#endif /* !INETWORK_HPP_ */
