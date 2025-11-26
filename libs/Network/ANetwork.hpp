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
#include "INetwork.hpp"
#include "../Packet/IPacket.hpp"
#include "../Buffer/IBuffer.hpp"

namespace net {

class ANetwork : public INetwork {
    public:

        virtual ~ANetwork() override = default;

        virtual void init(unsigned int port) override = 0;
        virtual void stop() override = 0;

        virtual int acceptConnection() override = 0;
        virtual void closeConnection(int connectionId) override = 0;
        virtual std::vector<int> getActiveConnections() const override = 0;
        virtual int getConnectionCount() const override = 0;

        virtual void sendTo(int connectionId, const IPacket &packet) override = 0;
        virtual void broadcast(const IPacket &packet) override = 0;
        virtual bool hasIncomingData() const override = 0;
        virtual std::shared_ptr<IPacket> receiveFrom(const int &connectionId) override = 0;

        virtual void sendData(const IPacket &data, size_t size) override = 0;
        virtual IPacket &receiveData(const IBuffer &buffer, size_t size) const override = 0;

        virtual void setConnectionCallback(std::function<void(int)> onConnect) override = 0;
        virtual void setDisconnectionCallback(std::function<void(int)> onDisconnect) override = 0;

    protected:
    private:
};

} // namespace net

#endif /* !ANETWORK_HPP_ */
