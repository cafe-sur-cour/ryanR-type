/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ANetwork
*/

#ifndef ANETWORK_HPP_
#define ANETWORK_HPP_

#include <vector>
#include <functional>
#include <memory>
#include "IEventLoop.hpp"
#include "INetworkEndpoint.hpp"
#include "../../common/interfaces/INetwork.hpp"
#include "../../common/interfaces/IPacketManager.hpp"
#include "../../common/interfaces/IBuffer.hpp"

namespace net {

class ANetwork : public INetwork {
    public:

        virtual ~ANetwork() override = default;
        virtual void init(uint16_t port, const std::string host) override = 0;
        virtual void stop() override = 0;
        virtual bool sendTo(const INetworkEndpoint& endpoint, std::vector<uint8_t> packet) override = 0;
        virtual bool broadcast(const std::vector<std::shared_ptr<INetworkEndpoint>>& endpoints, const std::vector<uint8_t>& data) override = 0;
        virtual bool hasIncomingData() const override = 0;
        virtual std::vector<uint8_t> receiveFrom(const uint8_t &connectionId) override = 0;
        virtual std::pair<std::shared_ptr<INetworkEndpoint>, std::vector<uint8_t>> receiveAny() override = 0;

        ConnectionState getConnectionState() const override;
        void setConnectionState(ConnectionState state) override;
        void setConnectionCallback(std::function<void(int)> onConnect) override;
        void setDisconnectionCallback(std::function<void(int)> onDisconnect) override;

    protected:
        std::shared_ptr<IEventLoop> _eventLoop;
        std::function<void(int)> _onConnectCallback;
        std::function<void(int)> _onDisconnectCallback;
        bool _isRunning;
        ConnectionState _connectionState;

};

} // namespace net

#endif /* !ANETWORK_HPP_ */
