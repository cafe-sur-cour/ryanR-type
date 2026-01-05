/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Server
*/

#include <string>
#include <memory>
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <utility>
#include <cstring>
#include <queue>

#include "Server.hpp"
#include "Constants.hpp"
#include "packet/DefaultPacketHandlers.hpp"
#include "../libs/Network/Unix/ServerNetwork.hpp"
#include "../../common/systems/systemManager/SystemManager.hpp"
#include "initResourcesManager/initResourcesManager.hpp"
#include "../common/Error/ServerErrror.hpp"
#include "../common/debug.hpp"
#include "../common/constants.hpp"
#include "Signal.hpp"

rserv::Server::Server() :
    _nextClientId(1), _sequenceNumber(1), _nextEntityId(1) {
    this->_clients = {};
    this->_config = nullptr;
    this->_network = nullptr;
    this->_buffer = nullptr;
    this->_packet = nullptr;
    this->_lobbyThreads = {};
    this->_lobbies = {};
    this->_config = std::make_shared<rserv::ServerConfig>();
}

rserv::Server::~Server() {
    if (this->getState() == 1) {
        try {
            this->stop();
        } catch (...) {
            std::cerr << "Error in destructor while stopping server" << std::endl;
        }
    }
    for (auto& lobbyStruct : this->_lobbyThreads) {
        if (lobbyStruct && lobbyStruct->_lobby) {
            try {
                lobbyStruct->_lobby->stop();
            } catch (...) {
                // Ignore exceptions in destructor
            }
        }
    }
    this->_lobbyThreads.clear();
    this->_lobbies.clear();
}


void rserv::Server::initRessourceManager(std::shared_ptr<Lobby> lobby) {
    auto lobbyResourceManager = initResourcesManager(
        std::shared_ptr<rserv::Server>(this),
        lobby
    );
    lobby->setResourceManager(lobbyResourceManager);
}

void rserv::Server::init() {
    if (!this->_config) {
        throw err::ServerError("[SERVER] Server configuration not set",
            err::ServerError::CONFIG_ERROR);
    }

    this->loadNetworkLibrary();
    this->loadBufferLibrary();
    this->loadPacketLibrary();
    this->_network->init(this->getPort(), this->_config->getIp());
    this->setState(0);
    debug::Debug::printDebug(this->_config->getIsDebug(),
        "[SERVER] Server initialized on port " + std::to_string(this->getPort()),
        debug::debugType::NETWORK, debug::debugLevel::INFO);
}

void rserv::Server::start() {
    if (this->getState() == 1) {
        std::cerr << "[SERVER] Info: Server is already running" << std::endl;
        return;
    }
    if (this->getState() == -1) {
        throw err::ServerError(
            "[SERVER] Error: init() must be called before start()",
            err::ServerError::INTERNAL_ERROR);
    }
    debug::Debug::printDebug(this->_config->getIsDebug(),
        "[SERVER] Starting server...",
        debug::debugType::NETWORK, debug::debugLevel::INFO);
    this->setState(1);

    Signal::setupSignalHandlers();
    while (this->getState() == 1 && !Signal::stopFlag) {
        this->processIncomingPackets();
        /* Replace the game stat packet by the monitoring of the threads and call lobby loop*/
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        if (std::cin.eof()) {
            debug::Debug::printDebug(this->_config->getIsDebug(),
                "EOF received (Ctrl+D pressed)",
                debug::debugType::NETWORK, debug::debugLevel::INFO);
            break;
        }
    }
    if (Signal::stopFlag) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "Termination signal received",
            debug::debugType::NETWORK, debug::debugLevel::INFO);
        this->stop();
    }
}

void rserv::Server::stop() {
    if (this->getState() == -1) {
        throw err::ServerError(
            "[SERVER] Error: init() must be called before stop()",
            err::ServerError::INTERNAL_ERROR);
    }
    if (this->getState() == 0) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Info: Server is not running.",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return;
    }
    Signal::stopFlag = true;
    for (auto lobby : this->_lobbies) {
        if (lobby) {
            try {
                lobby->stop();
            } catch (const std::exception& e) {
                std::cerr << "Error stopping lobby: " << e.what() << std::endl;
            } catch (...) {
                std::cerr << "Unknown error stopping lobby" << std::endl;
            }
        }
    }
    this->_lobbyThreads.clear();
    this->_lobbies.clear();
    this->_clientToLobby.clear();
    if (_network) {
        _network->stop();
    }
    this->setState(0);
    debug::Debug::printDebug(this->_config->getIsDebug(),
        "[SERVER] Server stopped.",
        debug::debugType::NETWORK, debug::debugLevel::INFO);
}

rserv::Server::operator int() const noexcept {
    return this->getState();
}

void rserv::Server::setConfig(std::shared_ptr<rserv::ServerConfig> config) {
    this->_config = config;
}

std::shared_ptr<rserv::ServerConfig> rserv::Server::getConfig() const {
    return this->_config;
}

uint16_t rserv::Server::getPort() const {
    return this->_config->getPort();
}

void rserv::Server::setPort(uint16_t port) {
    this->_config->setPort(port);
}

int rserv::Server::getState() const {
    return this->_config->getState();
}

void rserv::Server::setState(int state) {
    this->_config->setState(state);
}

std::shared_ptr<net::INetwork> rserv::Server::getNetwork() const {
    return _network;
}

void rserv::Server::setNetwork(std::shared_ptr<net::INetwork> network) {
    _network = network;
}

void rserv::Server::processIncomingPackets() {
    if (!_network) {
        std::cerr << "[SERVER] Warning: Network not initialized" << std::endl;
        return;
    }

    std::pair<std::shared_ptr<net::INetworkEndpoint>, std::vector<uint8_t>> received =
        _network->receiveAny();
    if (received.second.empty()) {
        return;
    }

    if (!this->_packet) {
        std::cerr << "[SERVER] Warning: Packet manager not initialized" << std::endl;
        return;
    }

    this->_packet->unpack(received.second);
    if (this->_packet->getType() == constants::PACKET_CONNECTION) {
            this->processConnections(std::make_pair(received.first, received.second));
    } else if (this->_packet->getType() == constants::PACKET_CLIENT_READY) {
        this->onPacketReceived(this->_packet->getIdClient(), *this->_packet);
    } else if (this->_packet->getType() == constants::PACKET_REQUEST_LOBBY) {
        this->requestCode(received.first);
    } else if (this->_packet->getType() == constants::PACKET_CONNECT_TO_LOBBY) {
        this->processConnectToLobby(std::make_pair(received.first, received.second));
    } else if (this->_packet->getType() == constants::PACKET_LOBBY_MASTER_REQUEST_START) {
        this->processMasterStart(std::make_pair(received.first, received.second));
    } else if (this->_packet->getType() == constants::PACKET_EVENT) {
        uint8_t idClient = this->_packet->getIdClient();
        auto it = this->_clientToLobby.find(idClient);
        if (it != this->_clientToLobby.end()) {
            it->second->enqueuePacket(received);
        } else {
            debug::Debug::printDebug(this->_config->getIsDebug(),
                "[SERVER] Received event packet from unknown client: " + std::to_string(idClient),
                debug::debugType::NETWORK, debug::debugLevel::WARNING);
        }
    } else {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Packet received of type "
            + std::to_string(static_cast<int>(this->_packet->getType()))
            + " from client: " + std::to_string(this->_packet->getIdClient()),
            debug::debugType::NETWORK, debug::debugLevel::INFO);
    }

    this->_packet->reset();
}

std::vector<uint8_t> rserv::Server::getConnectedClients() const {
    std::vector<uint8_t> clientIds;
    for (const auto &client : this->_clients) {
        clientIds.push_back(std::get<0>(client));
    }
    return clientIds;
}

std::vector<std::shared_ptr<net::INetworkEndpoint>>
    rserv::Server::getConnectedClientEndpoints() const {
    std::vector<std::shared_ptr<net::INetworkEndpoint>> endpoints;
    for (const auto &client : this->_clients) {
        endpoints.push_back(std::get<1>(client));
    }
    return endpoints;
}

size_t rserv::Server::getClientCount() const {
    return this->_clients.size();
}


void rserv::Server::onClientConnected(uint8_t idClient) {
    debug::Debug::printDebug(this->_config->getIsDebug(),
        "Client " + std::to_string(idClient) + " connected",
        debug::debugType::NETWORK, debug::debugLevel::INFO);

}

void rserv::Server::onClientDisconnected(uint8_t idClient) {
    debug::Debug::printDebug(this->_config->getIsDebug(),
        "Client " + std::to_string(idClient) + " disconnected",
        debug::debugType::NETWORK, debug::debugLevel::INFO);
}

void rserv::Server::onPacketReceived(
    uint8_t idClient, const pm::IPacketManager &packet) {
    debug::Debug::printDebug(this->_config->getIsDebug(),
        "Packet received from client " + std::to_string(idClient)
        + " of type " + std::to_string(static_cast<int>(packet.getType())),
        debug::debugType::NETWORK, debug::debugLevel::INFO);

}

uint32_t rserv::Server::getSequenceNumber() const {
    return this->_sequenceNumber;
}

std::shared_ptr<pm::IPacketManager> rserv::Server::getPacketManager() const {
    return this->_packet;
}

void rserv::Server::incrementSequenceNumber() {
    this->_sequenceNumber++;
}

std::shared_ptr<pm::IPacketManager> rserv::Server::createNewPacketManager() {
    createPacket_t createPacket = _packetloader.getSymbol("createPacketInstance");
    if (!createPacket) {
        throw err::ServerError("[Server] Cannot get createPacketInstance symbol", err::ServerError::LIBRARY_LOAD_FAILED);
    }
    auto packet = std::shared_ptr<pm::IPacketManager>(reinterpret_cast<pm::IPacketManager *>(createPacket()));
    if (!packet) {
        throw err::ServerError("[Server] Creating packet instance failed", err::ServerError::LIBRARY_LOAD_FAILED);
    }
    if (!rserv::packet::registerDefaultPacketHandlers(packet)) {
        throw err::ServerError("[Server] Registering default packet handlers failed", err::ServerError::LIBRARY_LOAD_FAILED);
    }
    return packet;
}

uint32_t rserv::Server::getNextEntityId() {
    return this->_nextEntityId++;
}
