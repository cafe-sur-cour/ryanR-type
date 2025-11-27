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

#include <memory>
#include "IServer.hpp"
#include "ServerConfig.hpp"
#include "../libs/Network/INetwork.hpp"
#include "../libs/Buffer/IBuffer.hpp"
#include "../common/DLLoader/DLLoader.hpp"
#include "../common/DLLoader/LoaderType.hpp"
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
            unsigned int getPort() const override;
            void setPort(unsigned int port) override;

            int getState() const override;
            void setState(int state) override;

            int getFd() const override;
            void setFd(int fd) override;
            operator int() const noexcept override;

            std::shared_ptr<net::INetwork> getNetwork() const override;
            void setNetwork(std::shared_ptr<net::INetwork> network) override;

            void onClientConnected(int idClient) override;
            void onClientDisconnected(int idClient) override;
            void onPacketReceived(int idClient, const IPacketManager &packet) override;

            void processConnections() override;
            void processIncomingPackets() override;

            void broadcastPacket() override;
            void sendToClient(int idClient ) override;
            std::vector<int> getConnectedClients() const override;
            size_t getClientCount() const override;

        private:
            void loadNetworkLibrary();
            void loadBufferLibrary();
            void loadPacketLibrary();
            DLLoader<createNetworkLib_t> _networloader;
            DLLoader<createBuffer_t> _bufferloader;
            DLLoader<createPacket_t> _packetloader;

            std::shared_ptr<ServerConfig> _config;
            std::shared_ptr<net::INetwork> _network;
            std::shared_ptr<IBuffer> _buffer;
            std::shared_ptr<IPacketManager> _packet;

    };
} // namespace rserv = r-type server

#endif /* !SERVER_HPP_ */
