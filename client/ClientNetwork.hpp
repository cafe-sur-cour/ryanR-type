/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ClientNetwork
*/

#include <memory>

#include "../common/DLLoader/DLLoader.hpp"
#include "../common/DLLoader/LoaderType.hpp"
#include "../libs/Network/INetwork.hpp"

#ifndef CLIENTNETWORK_HPP_
#define CLIENTNETWORK_HPP_

class ClientNetwork {
    public:
        ClientNetwork();
        ~ClientNetwork();

        void init();
        void start();
        void stop();

        int getPort() const;
        void setPort(int port);

        uint32_t getIp() const;
        void setIp(uint32_t ip);

        void sendData(const IPacket &data, size_t size);
        IPacket &receiveData(const IBuffer &buffer, size_t size) const;

        void loadNetworkLibrary();
        void loadBufferLibrary();
        void loadPacketLibrary();
    protected:
    private:
        DLLoader<createNetworkLib_t> _networloader;
        DLLoader<createBuffer_t> _bufferloader;
        DLLoader<createPacket_t> _packetloader;

        std::shared_ptr<net::INetwork> _network;
        std::shared_ptr<IBuffer> _buffer;
        std::shared_ptr<IPacket> _packet;

        int _port;
        uint32_t _ip;
};

#endif /* !CLIENTNETWORK_HPP_ */
