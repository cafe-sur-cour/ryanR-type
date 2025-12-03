/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ClientNetwork
*/
#include <cstring>

#include <thread>
#include <chrono>
#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <memory>

#include "ClientNetwork.hpp"
#include "../common/Error/ClientNetworkError.hpp"
#include "../common/Signal/Signal.hpp"

ClientNetwork::ClientNetwork() {
    this->_port = 0;
    this->_ip = "";
    this->_name = "Doof";
    this->_idClient = 0;
    this->_eventQueue = std::queue<NetworkEvent>();

    this->_network = nullptr;
    this->_buffer = nullptr;
    this->_packet = nullptr;
    this->_sequenceNumber = 0;
    this->_isConnected = false;
}

ClientNetwork::~ClientNetwork() {
    this->stop();
}

void ClientNetwork::init() {
    if (this->_port == 0 || this->_ip == "") {
        throw err::ClientNetworkError(
            "[ClientNetwork] Port and IP must be set before init()",
            err::ClientNetworkError::CONFIG_ERROR);
    }
    this->loadNetworkLibrary();
    this->loadBufferLibrary();
    this->loadPacketLibrary();
    this->_network->init(
        this->_port,
        this->_ip
    );
    this->_serverEndpoint = asio::ip::udp::endpoint(
        asio::ip::address::from_string(this->_ip),
        static_cast<uint16_t>(this->_port)
    );
}

void ClientNetwork::stop() {
    if (this->_network != nullptr
        && this->_networloader.getHandler() != nullptr) {
        this->_network.reset();
        this->_networloader.Close();
    }
    if (this->_buffer != nullptr
        && this->_bufferloader.getHandler() != nullptr) {
            this->_buffer.reset();
            this->_bufferloader.Close();
    }
    if (this->_packet != nullptr
        && this->_packetloader.getHandler() != nullptr) {
        this->_packet.reset();
        this->_packetloader.Close();
    }
}

/* Call this in a separate thread */
void ClientNetwork::start() {
    Signal::setupSignalHandlers();

    while (!Signal::stopFlag) {
        //   Hande the event queue
        std::vector<uint8_t> receivedData = this->_network->receiveFrom(this->_idClient);
        if (receivedData.size() > 0) {
            /* Transform the vector to packet to handle it correctly */
            std::cout << "[ClientNetwork] Received data of size: "
                      << receivedData.size() << std::endl;
            receivedData.clear();
        }
    }
    if (Signal::stopFlag) {
        this->disconnectionPacket();
        std::cout << "[Client] Received signal, stopping client" << std::endl;
        this->stop();
    }
}


uint16_t ClientNetwork::getPort() const {
    return _port;
}

void ClientNetwork::setPort(int port) {
    _port = static_cast<uint16_t>(port);
}

std::string ClientNetwork::getIp() const {
    return _ip;
}

void ClientNetwork::setIp(const std::string &ip) {
    _ip = ip;
}

void ClientNetwork::sendConnectionData(std::vector<uint8_t> packet) {
    if (!_network) {
        throw err::ClientNetworkError("[ClientNetwork] Network not initialized",
            err::ClientNetworkError::INTERNAL_ERROR);
    }
    this->_network->sendTo(this->_serverEndpoint, packet);
}

std::string ClientNetwork::getName() const {
    return this->_name;
}

void ClientNetwork::setName(const std::string &name) {
    this->_name = name;
}

uint8_t ClientNetwork::getIdClient() const {
    return this->_idClient;
}

void ClientNetwork::setIdClient(uint8_t idClient) {
    this->_idClient = idClient;
}

net::ConnectionState ClientNetwork::getConnectionState() const {
    if (!_network) {
        throw err::ClientNetworkError("[ClientNetwork] Network not initialized",
            err::ClientNetworkError::INTERNAL_ERROR);
    }
    return this->_network->getConnectionState();
}

void ClientNetwork::addToEventQueue(const NetworkEvent &event) {
    std::lock_guard<std::mutex> lock(this->_queueMutex);
    this->_eventQueue.push(event);
    this->_queueCond.notify_one();
    std::cout << "[ClientNetwork] Event added to queue. Queue size: "
          << this->_eventQueue.size() << std::endl;
}

bool ClientNetwork::getEventFromQueue(NetworkEvent &event) {
    std::unique_lock<std::mutex> lock(this->_queueMutex);
    if (this->_eventQueue.empty()) {
        return false;
    }
    event = this->_eventQueue.front();
    this->_eventQueue.pop();
    return true;
}


/* Packet Handling */
void ClientNetwork::connectionPacket() {
    if (!_network) {
        throw err::ClientNetworkError("[ClientNetwork] Network not initialized",
            err::ClientNetworkError::INTERNAL_ERROR);
    }
    std::vector<uint8_t> header = this->_packet->pack(this->_idClient,
        this->_sequenceNumber, 0x01);
    this->_network->sendTo(this->_serverEndpoint, header);
    /* Replace this with name */
    std::vector<uint64_t> payloadData = {0x01};
    for (size_t i = 0; i < this->_packet->formatString(this->_name).size(); i++)
        payloadData.push_back(this->_packet->formatString(this->_name)[i]);
    std::vector<uint8_t> payload = this->_packet->pack(payloadData);
    this->_network->sendTo(this->_serverEndpoint, payload);
    this->_sequenceNumber++;
}


void ClientNetwork::eventPacket(const constants::EventType &eventType,
    double depth, double direction) {
    if (!_network) {
        throw err::ClientNetworkError("[ClientNetwork] Network not initialized",
            err::ClientNetworkError::INTERNAL_ERROR);
    }
    std::vector<uint8_t> header =
        this->_packet->pack(this->_idClient, this->_sequenceNumber, 0x02);
    this->_network->sendTo(this->_serverEndpoint, header);

    std::vector<uint64_t> payloadData;
    payloadData.push_back(static_cast<uint64_t>(eventType));

    uint64_t depthBits;
    std::memcpy(&depthBits, &depth, sizeof(double));
    payloadData.push_back(depthBits);

    uint64_t dirBits;
    std::memcpy(&dirBits, &direction, sizeof(double));
    payloadData.push_back(dirBits);

    std::vector<uint8_t> payload = this->_packet->pack(payloadData);
    this->_network->sendTo(this->_serverEndpoint, payload);
    this->_sequenceNumber++;
}


void ClientNetwork::disconnectionPacket() {
    if (!_network) {
        throw err::ClientNetworkError("[ClientNetwork] Network not initialized",
            err::ClientNetworkError::INTERNAL_ERROR);
    }
    if (this->_idClient != 0) {
        std::cerr << "[ClientNetwork] Warning: Client ID is not set"
            << " cannot send disconnection packet" << std::endl;
        return;
    }
    if (!this->_packet) {
        std::cerr << "[ClientNetwork] Warning: Packet manager not initialized,"
        << " cannot send disconnection packet" << std::endl;
        return;
    }
    std::vector<uint8_t> header = this->_packet->pack(this->_idClient,
        this->_sequenceNumber, 0x03);
    this->_network->sendTo(this->_serverEndpoint, header);
    this->_sequenceNumber++;
}
