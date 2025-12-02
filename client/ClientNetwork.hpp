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
#include "../common/constants.hpp"

#ifndef CLIENTNETWORK_HPP_
#define CLIENTNETWORK_HPP_

class ClientNetwork {
    public:
        ClientNetwork();
        ~ClientNetwork();

        void init();
        void start();
        void stop();

        uint32_t getPort() const;
        void setPort(int port);

        std::string getIp() const;
        void setIp(const std::string &ip);

        void loadNetworkLibrary();
        void loadBufferLibrary();
        void loadPacketLibrary();

        void sendConnectionData(std::vector<uint8_t> packet);

        std::string getName() const;
        void setName(const std::string &name);

        uint8_t getIdClient() const;
        void setIdClient(uint8_t idClient);

        net::ConnectionState getConnectionState() const;

        /* Packet Handling */
        void eventPacket(const constants::EventType &eventType, double depth, double direction);
        void disconnectionPacket();
        void connectionPacket();

    protected:
    private:
        DLLoader<createNetworkLib_t> _networloader;
        DLLoader<createBuffer_t> _bufferloader;
        DLLoader<createPacket_t> _packetloader;

        std::shared_ptr<net::INetwork> _network;
        std::shared_ptr<IBuffer> _buffer;
        std::shared_ptr<pm::IPacketManager> _packet;

        uint32_t _sequenceNumber;
        uint32_t _port;
        std::string  _ip;
        std::string _name;
        uint8_t _idClient;
};

#endif /* !CLIENTNETWORK_HPP_ */
