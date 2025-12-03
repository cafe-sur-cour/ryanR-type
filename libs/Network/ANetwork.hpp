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
#include <asio.hpp>
#include "INetwork.hpp"
#include "../Packet/IPacketManager.hpp"
#include "../Buffer/IBuffer.hpp"

namespace net {

class ANetwork : public INetwork {
    public:

        virtual ~ANetwork() override = default;
        virtual void init(uint16_t port,const std::string host) override = 0;
        virtual void stop() override = 0;
        virtual uint8_t acceptConnection(asio::ip::udp::endpoint id, std::shared_ptr<pm::IPacketManager> packetManager) override = 0;
        virtual void sendTo(asio::ip::udp::endpoint id, std::vector<uint8_t> packet) override = 0;
        virtual void broadcast(const pm::IPacketManager &packet) override = 0;
        virtual bool hasIncomingData() const override = 0;
        virtual std::shared_ptr<pm::IPacketManager> receiveFrom(const uint8_t &connectionId) override = 0;

        ConnectionState getConnectionState() const override;
        void setConnectionState(ConnectionState state) override;

        void setConnectionCallback(std::function<void(int)> onConnect) override;
        void setDisconnectionCallback(std::function<void(int)> onDisconnect) override;
        std::vector<uint8_t> getActiveConnections() const override;
        size_t getConnectionCount() const override;
        void closeConnection(uint8_t connectionId) override;

    protected:
        std::shared_ptr<asio::io_context> _ioContext;
        std::shared_ptr<asio::ip::udp::socket> _socket;
        std::unordered_map<uint8_t, asio::ip::udp::endpoint> _clients;
        std::function<void(int)> _onConnectCallback;
        std::function<void(int)> _onDisconnectCallback;
        bool _isRunning;
        ConnectionState _connectionState;

};

} // namespace net

#endif /* !ANETWORK_HPP_ */
