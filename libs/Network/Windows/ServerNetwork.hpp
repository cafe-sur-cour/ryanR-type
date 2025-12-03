/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ServerNetwork
*/

#ifndef SERVERNETWORK_HPP_
#define SERVERNETWORK_HPP_

#include <asio.hpp>
#include <unordered_map>
#include <memory>
#include <queue>
#include <functional>
#include "../ANetwork.hpp"
#include "../../Buffer/IBuffer.hpp"
#include "../../Packet/IPacketManager.hpp"

namespace net {

class ServerNetwork :  public ANetwork {
    public:
        ServerNetwork();
        ~ServerNetwork() override;

        void init(uint16_t port, const std::string host) override;
        void stop() override;

        uint8_t acceptConnection(asio::ip::udp::endpoint id, std::shared_ptr<pm::IPacketManager> packetManager) override;

        void sendTo(asio::ip::udp::endpoint id, std::vector<uint8_t> packet) override;
        void broadcast(std::vector<uint8_t> data) override;
        bool hasIncomingData() const override;
        std::vector<uint8_t> receiveFrom(const uint8_t &connectionId) override;
        std::pair<asio::ip::udp::endpoint, std::vector<uint8_t>> receiveAny() override;

    private:
        std::queue<std::pair<int, std::shared_ptr<pm::IPacketManager>>> _incomingPackets;
        uint8_t _nextClientId;
        uint16_t _port;
};

} // namespace net

extern "C" {
    void *createNetworkInstance();
    int getType();
}

#endif /* !SERVERNETWORK_HPP_ */
