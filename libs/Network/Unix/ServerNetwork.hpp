/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** UnixServerNetwork
*/

#ifndef UNIXSERVERNETWORK_HPP_
#define UNIXSERVERNETWORK_HPP_

#include <asio.hpp>
#include <unordered_map>
#include <memory>
#include <queue>
#include <functional>
#include "../ANetwork.hpp"
#include "../../Buffer/IBuffer.hpp"
#include "../../Packet/IPacketManager.hpp"

namespace net {

class UnixServerNetwork : public ANetwork {
    public:
        UnixServerNetwork();
        ~UnixServerNetwork() override;

        void init(int port) override;
        void stop() override;

        int acceptConnection() override;

        void sendTo(int connectionId, const pm::IPacketManager &packet) override;
        void broadcast(const pm::IPacketManager &packet) override;
        bool hasIncomingData() const override;
        std::shared_ptr<pm::IPacketManager> receiveFrom(const int &connectionId) override;

        void sendData(const pm::IPacketManager &data, size_t size) override;
        pm::IPacketManager &receiveData(const IBuffer &buffer, size_t size) const override;

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

#endif /* !UNIXSERVERNETWORK_HPP_ */
