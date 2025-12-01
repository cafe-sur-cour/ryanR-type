/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** UnixClientNetwork
*/

#ifndef UNIXCLIENTNETWORK_HPP_
#define UNIXCLIENTNETWORK_HPP_

#include <asio.hpp>
#include <memory>
#include <string>
#include "../ANetwork.hpp"
#include "../../Buffer/IBuffer.hpp"
#include "../../Packet/IPacketManager.hpp"

namespace net {

class UnixClientNetwork : public ANetwork {
    public:
        UnixClientNetwork();
        ~UnixClientNetwork() override;

        void init(uint32_t port,const std::string host) override;
        void stop() override;

        uint8_t acceptConnection() override;

        void sendTo(int connectionId, const pm::IPacketManager &packet) override;
        void broadcast(const pm::IPacketManager &packet) override;
        bool hasIncomingData() const override;
        std::shared_ptr<pm::IPacketManager> receiveFrom(const int &connectionId) override;

    protected:
        // Client-specific methods
        void disconnect();
        bool isConnected() const;
    private:
        asio::ip::udp::endpoint _serverEndpoint;
        bool _connected;
};

} // namespace net

extern "C" {
    void *createNetworkInstance();
    int getType();
}

#endif /* !UNIXCLIENTNETWORK_HPP_ */
