/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ClientNetwork
*/


#ifndef CLIENTNETWORK_HPP_
#define CLIENTNETWORK_HPP_

#include <memory>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include "../common/DLLoader/DLLoader.hpp"
#include "../common/DLLoader/LoaderType.hpp"
#include "../libs/Network/INetwork.hpp"
#include "../common/constants.hpp"

struct NetworkEvent {
    constants::EventType eventType;
    double depth;
    double direction;
};

class ClientNetwork {
    public:
        ClientNetwork();
        ~ClientNetwork();

        void init();
        void start();
        void stop();

        uint16_t getPort() const;
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

        void addToEventQueue(const NetworkEvent &event);
        bool getEventFromQueue(NetworkEvent &event);

        std::atomic<bool> _isConnected;
    protected:
        void handlePacketType(uint8_t type);
    private:
        typedef void (ClientNetwork::*PacketHandler)();
        PacketHandler _packetHandlers[10];

        void handleNoOp();
        void handleConnectionAcceptation();
        void handleDisconnection();
        void handleEvent();
        void handleGameState();
        void handleMapSend();
        void handleEndMap();
        void handleEndGame();
        void handleCanStart();
        DLLoader<createNetworkLib_t> _networloader;
        DLLoader<createBuffer_t> _bufferloader;
        DLLoader<createPacket_t> _packetloader;

        std::shared_ptr<net::INetwork> _network;
        std::shared_ptr<IBuffer> _receptionBuffer;
        std::shared_ptr<pm::IPacketManager> _packet;

        uint32_t _sequenceNumber;
        uint16_t _port;
        std::string  _ip;
        std::string _name;
        uint8_t _idClient;
        asio::ip::udp::endpoint _serverEndpoint;

        std::queue<NetworkEvent> _eventQueue;
        std::mutex _queueMutex;
        std::condition_variable _queueCond;
};

#endif /* !CLIENTNETWORK_HPP_ */
