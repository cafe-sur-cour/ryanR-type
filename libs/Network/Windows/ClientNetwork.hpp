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
#include "../NetworkEndpoint.hpp"
#include "../../Buffer/IBuffer.hpp"
#include "../../Packet/IPacketManager.hpp"

namespace net {

class INetworkSocket;

class ClientNetwork : public ANetwork {
    public:
        ClientNetwork();
        ~ClientNetwork() override;

        void init(uint16_t port, const std::string host) override;
        void stop() override;
        void disconnect() override;
        bool isConnected() const override;

        bool sendTo(const NetworkEndpoint& endpoint, std::vector<uint8_t> packet) override;
        bool broadcast(const std::vector<NetworkEndpoint>& endpoints, std::vector<uint8_t> data) override;
        bool hasIncomingData() const override;
        std::vector<uint8_t> receiveFrom(const uint8_t &connectionId) override;
        std::pair<NetworkEndpoint, std::vector<uint8_t>> receiveAny() override;

    private:
        NetworkEndpoint _serverEndpoint;
        bool _connected;
        std::shared_ptr<INetworkSocket> _socket;
};

} // namespace net

extern "C" {
    void *createNetworkInstance();
    int getType();
}

#endif /* !CLIENTNETWORK_HPP_ */

        std::pair<NetworkEndpoint, std::vector<uint8_t>> receiveAny() override;

    protected:
        void disconnect();
        bool isConnected() const;

    private:
        asio::ip::udp::endpoint _serverEndpoint;
        bool _connected;
        std::shared_ptr<asio::ip::udp::socket> _socket;
};

} // namespace net

extern "C" {
    NETWORK_API void *createNetworkInstance();
    NETWORK_API int getType();
}

#endif /* !WINDOWSNETWORK_HPP_ */
