/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Header
*/

#ifndef SERVER_HPP_
    #define SERVER_HPP_

#ifdef _WIN32
    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT 0x0A00
    #endif

    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
#endif

#include <queue>
#include <map>
#include <memory>
#include "IServer.hpp"
#include "ServerConfig.hpp"
#include "../libs/Network/INetwork.hpp"
#include "../libs/Buffer/IBuffer.hpp"
#include "../common/DLLoader/DLLoader.hpp"
#include "../common/DLLoader/LoaderType.hpp"
#include "../common/constants.hpp"
#include "Signal.hpp"

namespace rserv {
    class Server : public IServer {
        public:
            Server();
            ~Server();

            void init() override;
            void start() override;
            void stop() override;

            void setConfig(std::shared_ptr<ServerConfig> config) override;
            std::shared_ptr<ServerConfig> getConfig() const override;
            uint16_t getPort() const override;
            void setPort(uint16_t port) override;

            int getState() const override;
            void setState(int state) override;

            int getFd() const override;
            void setFd(int fd) override;
            operator int() const noexcept override;

            std::shared_ptr<net::INetwork> getNetwork() const override;
            void setNetwork(std::shared_ptr<net::INetwork> network) override;

            void onClientConnected(uint8_t idClient) override;
            void onClientDisconnected(uint8_t idClient) override;
            void onPacketReceived(uint8_t idClient, const pm::IPacketManager &packet) override;

            void processIncomingPackets() override;
            bool processConnections(asio::ip::udp::endpoint id) override;
            bool processDisconnections(uint8_t idClient) override;
            bool processEvents(uint8_t idClient) override;
            bool processEndOfGame(uint8_t idClient) override;

            std::vector<uint8_t> getConnectedClients() const override;
            std::vector<asio::ip::udp::endpoint> getConnectedClientEndpoints() const override;
            size_t getClientCount() const override;

            std::shared_ptr<std::queue<std::tuple<uint8_t, constants::EventType, double, double>>> getEventQueue() override;
            bool hasEvents() const override;

        private:
            void loadNetworkLibrary();
            void loadBufferLibrary();
            void loadPacketLibrary();
            DLLoader<createNetworkLib_t> _networloader;
            DLLoader<createBuffer_t> _bufferloader;
            DLLoader<createPacket_t> _packetloader;
            uint8_t _nextClientId;
            uint32_t _sequenceNumber;
            std::vector<std::tuple<uint8_t, asio::ip::udp::endpoint, std::string>> _clients;

            std::shared_ptr<ServerConfig> _config;
            std::shared_ptr<net::INetwork> _network;
            std::shared_ptr<IBuffer> _buffer;
            std::shared_ptr<pm::IPacketManager> _packet;
            std::shared_ptr<std::queue<std::tuple<uint8_t, constants::EventType, double, double>>> _eventQueue;

    };
} // namespace rserv = r-type server

#endif /* !SERVER_HPP_ */
