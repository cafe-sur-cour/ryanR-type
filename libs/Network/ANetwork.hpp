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
        virtual void init(int port) override = 0;
        virtual void stop() override = 0;
        virtual int acceptConnection() override = 0;
        virtual void sendTo(int connectionId, const pm::IPacketManager &packet) override = 0;
        virtual void broadcast(const pm::IPacketManager &packet) override = 0;
        virtual bool hasIncomingData() const override = 0;
        virtual std::shared_ptr<pm::IPacketManager> receiveFrom(const int &connectionId) override = 0;
        virtual void sendData(const pm::IPacketManager &data, size_t size) override = 0;
        virtual pm::IPacketManager &receiveData(const IBuffer &buffer, size_t size) const override = 0;

        void setConnectionCallback(std::function<void(int)> onConnect) override;
        void setDisconnectionCallback(std::function<void(int)> onDisconnect) override;
        std::vector<int> getActiveConnections() const override;
        size_t getConnectionCount() const override;
        void closeConnection(int connectionId) override;

    protected:
        std::shared_ptr<asio::io_context> _ioContext;
        std::shared_ptr<asio::ip::udp::socket> _socket;
        std::unordered_map<int, asio::ip::udp::endpoint> _clients;
        std::function<void(int)> _onConnectCallback;
        std::function<void(int)> _onDisconnectCallback;
        bool _isRunning;

};

} // namespace net

#endif /* !ANETWORK_HPP_ */
