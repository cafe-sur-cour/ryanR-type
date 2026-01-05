/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** UnixServerNetwork
*/

#ifndef UNIXSERVERNETWORK_HPP_
#define UNIXSERVERNETWORK_HPP_

#include <unordered_map>
#include <memory>
#include <queue>
#include <functional>
#include "../ANetwork.hpp"
#include "../../../common/interfaces/INetworkEndpoint.hpp"
#include "../../../common/interfaces/IBuffer.hpp"
#include "../../../common/interfaces/IPacketManager.hpp"

namespace net {

class INetworkSocket;

class UnixServerNetwork : public ANetwork {
    public:
        UnixServerNetwork();
        ~UnixServerNetwork() override;

        void init(uint16_t port, const std::string host) override;
        void stop() override;

        bool sendTo(const INetworkEndpoint& endpoint, std::vector<uint8_t> packet) override;
        bool broadcast(const std::vector<std::shared_ptr<INetworkEndpoint>>& endpoints, const std::vector<uint8_t>& data) override;
        bool hasIncomingData() const override;
        std::vector<uint8_t> receiveFrom(const uint8_t &connectionId) override;
        std::pair<std::shared_ptr<INetworkEndpoint>, std::vector<uint8_t>> receiveAny() override;

    private:
        std::queue<std::pair<int, std::shared_ptr<pm::IPacketManager>>> _incomingPackets;
        uint16_t _port;
        std::shared_ptr<INetworkSocket> _socket;
};

} // namespace net

extern "C" {
    void *createNetworkInstance();
    int getType();
}

#endif /* !UNIXSERVERNETWORK_HPP_ */
