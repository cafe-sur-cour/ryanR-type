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

        void init(uint16_t port,const std::string host) override;
        void stop() override;

        bool acceptConnection(asio::ip::udp::endpoint id, std::shared_ptr<pm::IPacketManager> packetManager, uint8_t clientId) override;

        bool sendTo(asio::ip::udp::endpoint id, std::vector<uint8_t> packet) override;
        void broadcast(std::vector<uint8_t> data) override;
        bool hasIncomingData() const override;
        std::vector<uint8_t> receiveFrom(const uint8_t &connectionId) override;
        std::pair<asio::ip::udp::endpoint, std::vector<uint8_t>> receiveAny() override;


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
