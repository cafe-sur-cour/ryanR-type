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
#include <memory>

#include "ClientNetwork.hpp"
#include "../common/Error/ClientNetworkError.hpp"
#include "../common/Signal/Signal.hpp"

ClientNetwork::ClientNetwork() {
    this->_port = 0;
    this->_ip = "";
    this->_name = ""; //  init with a default name
    this->_idClient = 0;
    this->_eventQueue = std::queue<NetworkEvent>();

    this->_network = nullptr;
    this->_buffer = nullptr;
    this->_packet = nullptr;
    this->_sequenceNumber = 0;
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
        // std::unique_lock<std::mutex> lock(_queueMutex);
        // _queueCond.wait(lock, [this]() { return !_eventQueue.empty() || Signal::stopFlag; });
        // if (Signal::stopFlag) break;
        // while (!_eventQueue.empty()) {
        //     NetworkEvent event = _eventQueue.front();
        //     _eventQueue.pop();
        //     lock.unlock();

        //     this->eventPacket(event.eventType, event.depth, event.direction);

        //     lock.lock();
        // }
        this->_network->receiveFrom(this->_idClient);
    }
    if (Signal::stopFlag) {
        this->disconnectionPacket();
        std::cout << "[Client] Received signal, stopping client" << std::endl;
        this->stop();
    }
}


uint32_t ClientNetwork::getPort() const {
    return _port;
}

void ClientNetwork::setPort(int port) {
    _port = static_cast<uint32_t>(port);
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
    this->_network->sendTo(0, packet);
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
    std::vector<uint8_t> header = this->_packet->pack(this->_idClient, this->_sequenceNumber, 0x01);
    this->_network->sendTo(this->_idClient, header);
    /* Replace this with name */
    std::vector<uint64_t> payloadData = {0x01, 'A', 'L', 'B', 'A', 'N', 'E', '\0', '\0', '\r', '\n'};
    std::vector<uint8_t> payload = this->_packet->pack(payloadData);
    this->_network->sendTo(this->_idClient, payload);
    this->_sequenceNumber++;
}


void ClientNetwork::eventPacket(const constants::EventType &eventType, double depth, double direction)
{
    if (!_network) {
        throw err::ClientNetworkError("[ClientNetwork] Network not initialized",
            err::ClientNetworkError::INTERNAL_ERROR);
    }
    std::vector<uint8_t> header =
        this->_packet->pack(this->_idClient, this->_sequenceNumber, 0x02);
    this->_network->sendTo(this->_idClient, header);

    std::vector<uint64_t> payloadData;
    payloadData.push_back(static_cast<uint64_t>(eventType));

    uint64_t depthBits;
    std::memcpy(&depthBits, &depth, sizeof(double));
    payloadData.push_back(depthBits);

    uint64_t dirBits;
    std::memcpy(&dirBits, &direction, sizeof(double));
    payloadData.push_back(dirBits);

    std::vector<uint8_t> payload = this->_packet->pack(payloadData);
    this->_network->sendTo(this->_idClient, payload);
    this->_sequenceNumber++;
}


void ClientNetwork::disconnectionPacket() {
    if (!_network) {
        throw err::ClientNetworkError("[ClientNetwork] Network not initialized",
            err::ClientNetworkError::INTERNAL_ERROR);
    }
    std::vector<uint8_t> header = this->_packet->pack(this->_idClient, this->_sequenceNumber, 0x03);
    this->_network->sendTo(this->_idClient, header);
    this->_sequenceNumber++;
}
