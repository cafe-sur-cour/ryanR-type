/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** WindowsNetwork
*/

#ifndef WINDOWSNETWORK_HPP_
    #define WINDOWSNETWORK_HPP_

#ifdef _WIN32
    #ifdef NETWORK_EXPORTS
        #define NETWORK_API __declspec(dllexport)
    #else
        #define NETWORK_API __declspec(dllimport)
    #endif
#else
    #define NETWORK_API
#endif

#ifndef ASIO_STANDALONE
    #define ASIO_STANDALONE
#endif

#ifndef _WIN32_WINNT
    #define _WIN32_WINNT 0x0A00
#endif

#include "../ANetwork.hpp"
#include "../../Packet/IPacketManager.hpp"
#include "../../Buffer/IBuffer.hpp"

namespace net {

class WindowsNetwork : public ANetwork {
    public:
        WindowsNetwork();
        ~WindowsNetwork() override;

        void init(int port) override;
        void stop() override;

        int acceptConnection() override;
        void closeConnection(int connectionId) override;
        std::vector<int> getActiveConnections() const override;
        size_t getConnectionCount() const override;

        void sendTo(int connectionId, const pm::IPacketManager &packet) override;
        void broadcast(const pm::IPacketManager &packet) override;
        bool hasIncomingData() const override;
        std::shared_ptr<pm::IPacketManager> receiveFrom(const int &connectionId) override;

        void setConnectionCallback(std::function<void(int)> onConnect) override;
        void setDisconnectionCallback(std::function<void(int)> onDisconnect) override;

    protected:
    private:
};

} // namespace net

extern "C" {
    NETWORK_API void *createNetworkInstance();
    NETWORK_API int getType();
}

#endif /* !WINDOWSNETWORK_HPP_ */
