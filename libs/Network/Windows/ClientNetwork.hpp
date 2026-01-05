/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ClientNetwork
*/

#ifndef CLIENTNETWORK_HPP_
#define CLIENTNETWORK_HPP_

#include <memory>
#include <string>
#include "../ANetwork.hpp"
#include "../../../common/interfaces/INetworkEndpoint.hpp"
#include "../../../common/interfaces/IBuffer.hpp"
#include "../../../common/interfaces/IPacketManager.hpp"

namespace net {

class INetworkSocket;

class ClientNetwork : public ANetwork {
    public:
        ClientNetwork();
        ~ClientNetwork() override;

        void init(uint16_t port, const std::string host) override;
        void stop() override;

        bool sendTo(const INetworkEndpoint& endpoint, std::vector<uint8_t> packet) override;
        bool broadcast(const std::vector<std::shared_ptr<INetworkEndpoint>>& endpoints, const std::vector<uint8_t>& data) override;
        bool hasIncomingData() const override;
        std::vector<uint8_t> receiveFrom(const uint8_t &connectionId) override;
        std::pair<std::shared_ptr<INetworkEndpoint>, std::vector<uint8_t>> receiveAny() override;

    protected:
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

#endif /* !CLIENTNETWORK_HPP_ */
