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
#include "../Packet/IPacketManager.hpp"
#include "../Buffer/IBuffer.hpp"

namespace net {

class INetwork {
    public:

        virtual ~INetwork() = default;

        virtual void init(int port) = 0;
        virtual void stop() = 0;

        virtual int acceptConnection() = 0;
        virtual void closeConnection(int connectionId) = 0;
        virtual std::vector<int> getActiveConnections() const = 0;
        virtual size_t getConnectionCount() const = 0;

        virtual void sendTo(int connectionId, const pm::IPacketManager &packet) = 0;
        virtual void broadcast(const pm::IPacketManager &packet) = 0;
        virtual bool hasIncomingData() const = 0;
        virtual std::shared_ptr<pm::IPacketManager> receiveFrom(const int &connectionId) = 0;

        virtual void setConnectionCallback(std::function<void(int)> onConnect) = 0;
        virtual void setDisconnectionCallback(std::function<void(int)> onDisconnect) = 0;

    protected:
    private:
};

} // namespace net

#endif /* !INETWORK_HPP_ */
