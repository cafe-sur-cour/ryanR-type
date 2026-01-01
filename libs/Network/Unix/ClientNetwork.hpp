/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** UnixClientNetwork
*/

#ifndef UNIXCLIENTNETWORK_HPP_
#define UNIXCLIENTNETWORK_HPP_

#include <memory>
#include <string>
#include <asio.hpp>
#include "../ANetwork.hpp"
#include "../NetworkEndpoint.hpp"
#include "../../Buffer/IBuffer.hpp"
#include "../../Packet/IPacketManager.hpp"

namespace net {

class UnixClientNetwork : public ANetwork {
    public:
        UnixClientNetwork();
        ~UnixClientNetwork() override;

        void init(uint16_t port,const std::string host) override;
        void stop() override;

        bool sendTo(const NetworkEndpoint& endpoint, std::vector<uint8_t> packet) override;
        bool broadcast(const std::vector<NetworkEndpoint>& endpoints, std::vector<uint8_t> data) override;
        bool hasIncomingData() const override;
        std::vector<uint8_t> receiveFrom(const uint8_t &connectionId) override;
        std::pair<NetworkEndpoint, std::vector<uint8_t>> receiveAny() override;


    protected:
        // Client-specific methods
        void disconnect();
        bool isConnected() const;
    private:
        asio::ip::udp::endpoint _serverEndpoint;
        bool _connected;
        std::shared_ptr<asio::ip::udp::socket> _socket;
};

} // namespace net

extern "C" {
    void *createNetworkInstance();
    int getType();
}

#endif /* !UNIXCLIENTNETWORK_HPP_ */
