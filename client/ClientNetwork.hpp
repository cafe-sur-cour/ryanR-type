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
#include "../common/resourceManager/ResourceManager.hpp"
#include "../common/gsm/IGameStateMachine.hpp"

struct NetworkEvent {
    constants::EventType eventType;
    double depth;
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
        std::shared_ptr<net::INetwork> getNetwork() const;

        void setDebugMode(bool isDebug);
        bool isDebugMode() const;

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
        void eventPacket(const constants::EventType &eventType, double depth);
        void disconnectionPacket();
        void connectionPacket();
        void sendReady();

        void addToEventQueue(const NetworkEvent &event);

        bool isConnected() const;
        bool isReady() const;
        std::atomic<bool> _isConnected;
        std::atomic<bool> _ready;

        void setResourceManager(std::shared_ptr<ResourceManager> resourceManager);
        void setGameStateMachine(std::shared_ptr<gsm::IGameStateMachine> gsm);
        std::shared_ptr<gsm::IGameStateMachine> getGameStateMachine() const;
    protected:
        std::pair<int, std::chrono::steady_clock::time_point> tryConnection(const int maxRetries, int retryCount, std::chrono::steady_clock::time_point lastRetryTime);
        void handlePacketType(uint8_t type);
    private:
        typedef void (ClientNetwork::*PacketHandler)();
        PacketHandler _packetHandlers[10];

        void handleNoOp();
        void handleConnectionAcceptation();
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
        std::shared_ptr<IBuffer> _sendBuffer;
        std::shared_ptr<pm::IPacketManager> _packet;

        std::shared_ptr<ResourceManager> _resourceManager;
        std::shared_ptr<gsm::IGameStateMachine> _gsm;

        uint32_t _sequenceNumber;
        uint16_t _port;
        std::string  _ip;
        std::string _name;
        std::vector<std::string> _clientNames;
        bool _isDebug;


        uint8_t _idClient;
        asio::ip::udp::endpoint _serverEndpoint;

        std::queue<NetworkEvent> _eventQueue;
        std::mutex _queueMutex;
        std::condition_variable _queueCond;
};

#endif /* !CLIENTNETWORK_HPP_ */
