/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ServerNetwork
*/

#ifndef SERVERNETWORK_HPP_
#define SERVERNETWORK_HPP_

#include <unordered_map>
#include <memory>
#include <queue>
#include <functional>
#include <asio.hpp>
#include "../ANetwork.hpp"
#include "../NetworkEndpoint.hpp"
#include "../../Buffer/IBuffer.hpp"
#include "../../Packet/IPacketManager.hpp"

namespace net {

class ServerNetwork :  public ANetwork {
    public:
        ServerNetwork();
        ~ServerNetwork() override;

        void init(uint16_t port, const std::string host) override;
        void stop() override;


        bool sendTo(const NetworkEndpoint& endpoint, std::vector<uint8_t> packet) override;
        bool broadcast(const std::vector<NetworkEndpoint>& endpoints, std::vector<uint8_t> data) override;
        bool hasIncomingData() const override;
        std::vector<uint8_t> receiveFrom(const uint8_t &connectionId) override;
        std::pair<NetworkEndpoint, std::vector<uint8_t>> receiveAny() override;

    private:
        std::queue<std::pair<int, std::shared_ptr<pm::IPacketManager>>> _incomingPackets;
        std::unordered_map<uint8_t, asio::ip::udp::endpoint> _clients;
        uint8_t _nextClientId;
        uint16_t _port;
        std::shared_ptr<asio::ip::udp::socket> _socket;
};

} // namespace net

extern "C" {
    void *createNetworkInstance();
    int getType();
}

#endif /* !SERVERNETWORK_HPP_ */
