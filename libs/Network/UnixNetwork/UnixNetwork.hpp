/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** UnixNetwork
*/

#ifndef UnixNetwork_HPP_
#define UnixNetwork_HPP_

#include <asio.hpp>
#include <unordered_map>
#include <memory>
#include <queue>
#include <functional>
#include "../ANetwork.hpp"
#include "../../Buffer/IBuffer.hpp"
#include "../../Packet/IPacket.hpp"

namespace net {

class UnixNetwork : public ANetwork {
    public:
        UnixNetwork();
        ~UnixNetwork() override;


        void init(int port) override;
        void stop() override;

        int acceptConnection() override;
        void closeConnection(int connectionId) override;
        std::vector<int> getActiveConnections() const override;
        size_t getConnectionCount() const override;

        void sendTo(int connectionId, const IPacket &packet) override;
        void broadcast(const IPacket &packet) override;
        bool hasIncomingData() const override;
        std::shared_ptr<IPacket> receiveFrom(const int &connectionId) override;

        void sendData(const IPacket &data, size_t size) override;
        IPacket &receiveData(const IBuffer &buffer, size_t size) const override;

        void setConnectionCallback(std::function<void(int)> onConnect) override;
        void setDisconnectionCallback(std::function<void(int)> onDisconnect) override;

    protected:
    private:
        std::shared_ptr<asio::io_context> _ioContext;
        std::shared_ptr<asio::ip::udp::socket> _socket;
        std::unordered_map<int, asio::ip::udp::endpoint> _clients;
        std::queue<std::pair<int, std::shared_ptr<IPacket>>> _incomingPackets;
        std::function<void(int)> _onConnectCallback;
        std::function<void(int)> _onDisconnectCallback;
        int _nextClientId;
        int _port;
        bool _isRunning;
};

} // namespace net

extern "C" {
    void *createNetworkInstance();
    int getType();
}

#endif /* !UnixNetwork_HPP_ */
