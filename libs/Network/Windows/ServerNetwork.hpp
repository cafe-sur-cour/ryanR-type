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

        void init(uint32_t port,const std::string host) override;
        void stop() override;

        uint8_t acceptConnection() override;

        void sendTo(int connectionId, std::vector<uint8_t> data) override;
        void broadcast(std::vector<uint8_t> data) override;
        bool hasIncomingData() const override;
        std::shared_ptr<pm::IPacketManager> receiveFrom(const int &connectionId) override;

    private:
        std::queue<std::pair<int, std::shared_ptr<pm::IPacketManager>>> _incomingPackets;
        int _nextClientId;
        int _port;
};

} // namespace net

extern "C" {
    void *createNetworkInstance();
    int getType();
}

#endif /* !SERVERNETWORK_HPP_ */
