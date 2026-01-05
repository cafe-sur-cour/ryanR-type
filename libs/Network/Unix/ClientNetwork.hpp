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
#include "../ANetwork.hpp"
#include "../../../common/interfaces/INetworkEndpoint.hpp"
#include "../../../common/interfaces/IBuffer.hpp"
#include "../../../common/interfaces/IPacketManager.hpp"

namespace net {

class INetworkSocket;

class UnixClientNetwork : public ANetwork {
    public:
        UnixClientNetwork();
        ~UnixClientNetwork() override;

        void init(uint16_t port, const std::string host) override;
        void stop() override;

        bool sendTo(const INetworkEndpoint& endpoint, std::vector<uint8_t> packet) override;
        bool broadcast(const std::vector<std::shared_ptr<INetworkEndpoint>>& endpoints, const std::vector<uint8_t>& data) override;
        bool hasIncomingData() const override;
        std::vector<uint8_t> receiveFrom(const uint8_t &connectionId) override;
        std::pair<std::shared_ptr<INetworkEndpoint>, std::vector<uint8_t>> receiveAny() override;

    protected:
        // Client-specific methods
        void disconnect();
        bool isConnected() const;
    private:
        std::shared_ptr<INetworkEndpoint> _serverEndpoint;
        bool _connected;
        std::shared_ptr<INetworkSocket> _socket;
};

} // namespace net

extern "C" {
    void *createNetworkInstance();
    int getType();
}

#endif /* !UNIXCLIENTNETWORK_HPP_ */
