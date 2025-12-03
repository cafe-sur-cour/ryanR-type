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

#include <asio.hpp>
#include <memory>
#include <string>
#include "../ANetwork.hpp"
#include "../../Buffer/IBuffer.hpp"
#include "../../Packet/IPacketManager.hpp"


namespace net {

class WindowsNetwork : public ANetwork {
    public:
        WindowsNetwork();
        ~WindowsNetwork() override;

        void init(uint16_t port, const std::string host) override;
        void stop() override;

        uint8_t acceptConnection(asio::ip::udp::endpoint id, std::shared_ptr<pm::IPacketManager> packetManager) override;
        void sendTo(asio::ip::udp::endpoint id, std::vector<uint8_t> packet) override;
        void broadcast(const pm::IPacketManager &packet) override;
        bool hasIncomingData() const override;
        std::vector<uint8_t> receiveFrom(const uint8_t &connectionId) override;
        std::pair<asio::ip::udp::endpoint, std::vector<uint8_t>> receiveAny() override;

    protected:
    private:
};

} // namespace net

extern "C" {
    NETWORK_API void *createNetworkInstance();
    NETWORK_API int getType();
}

#endif /* !WINDOWSNETWORK_HPP_ */
