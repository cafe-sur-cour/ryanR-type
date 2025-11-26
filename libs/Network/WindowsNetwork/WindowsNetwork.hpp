/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** WindowsNetwork
*/

#ifndef WINDOWSNETWORK_HPP_
#define WINDOWSNETWORK_HPP_

#include "../ANetwork.hpp"
#include "../../Packet/IPacket.hpp"
#include "../../Buffer/IBuffer.hpp"

namespace net {

class WindowsNetwork : public ANetwork {
    public:
        WindowsNetwork();
        ~WindowsNetwork() override;

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
};

} // namespace net

extern "C" {
    void *createNetworkInstance();
    int getType();
}

#endif /* !WINDOWSNETWORK_HPP_ */
