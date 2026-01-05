/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ServerReceivePacket
*/

#include <vector>
#include <iostream>
#include <string>
#include <memory>
#include <thread>

#include "Server.hpp"
#include "Constants.hpp"
#include "../common/debug.hpp"
#include "../common/components/tags/PlayerTag.hpp"
#include "../common/ECS/entity/registry/Registry.hpp"

bool rserv::Server::processConnections(std::pair<asio::ip::udp::endpoint,
    std::vector<uint8_t>> client) {
    if (!_network) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Warning: Network not initialized",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }
    std::string name = "";
    if (client.second.size() > HEADER_SIZE)
        name = std::string(client.second.begin() + HEADER_SIZE, client.second.end());

    this->connectionPacket(client.first);
    this->_clients.push_back(std::make_tuple(this->_nextClientId, client.first, name));
    this->_clientsReady[this->_nextClientId] = false;
    debug::Debug::printDebug(this->_config->getIsDebug(), "[SERVER] Set client " +
        std::to_string(this->_nextClientId) + " ready to false",
        debug::debugType::NETWORK, debug::debugLevel::INFO);
    this->_nextClientId++;
    return true;
}

bool rserv::Server::processDisconnections(uint8_t idClient) {
    for (auto &client : this->_clients) {
        if (std::get<0>(client) == idClient) {
            this->_clients.erase(
                std::remove(this->_clients.begin(), this->_clients.end(), client),
                this->_clients.end());
            this->_nextClientId--;
            debug::Debug::printDebug(this->_config->getIsDebug(),
                "Client " + std::to_string(idClient)
                + " disconnected and removed from the lobby",
                debug::debugType::NETWORK, debug::debugLevel::INFO);
            return true;
        }
    }
    return false;
}

bool rserv::Server::requestCode(asio::ip::udp::endpoint endpoint) {
    if (!this->_network) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Warning: Network not initialized",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }
    if (this->sendCodeLobbyPacket(endpoint) == false) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Warning: Failed to send lobby code",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }
    return true;
}

bool rserv::Server::processConnectToLobby(std::pair<asio::ip::udp::endpoint,
    std::vector<uint8_t>> payload) {
    /* Verify Network */
    if (!this->_network) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Warning: Network not initialized",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }
    /* Verify that lobby exists */
    std::string lobbyCode = "";
    if (payload.second.size() > HEADER_SIZE)
        lobbyCode = std::string(payload.second.begin() + HEADER_SIZE, payload.second.end());
    bool lobbyExists = false;
    for (const auto &lobby : this->_lobbyThreads) {
        if (lobby->_lobbyCode == lobbyCode) {
            if (lobby->_clients.size() > constants::MAX_CLIENT_PER_LOBBY) {
                debug::Debug::printDebug(this->_config->getIsDebug(),
                    "[SERVER] Warning: Maximum clients reached for lobby",
                    debug::debugType::NETWORK, debug::debugLevel::WARNING);
                break;
            }
            lobbyExists = true;
            break;
        }
   }
    /* Send succesfull or fail connect */
    this->lobbyConnectValuePacket(payload.first, lobbyExists);
    /* Add client to lobby */
    std::tuple<uint8_t, asio::ip::udp::endpoint, std::string> clientToAdd;
    for (const auto &client : this->_clients) {
        if (std::get<1>(client) == payload.first) {
            clientToAdd = client;
            break;
        }
    }
    if (lobbyExists) {
       for (auto &lobby : this->_lobbyThreads) {
            if (lobby->_lobbyCode == lobbyCode) {
                lobby->_clients.push_back(clientToAdd);
                debug::Debug::printDebug(this->_config->getIsDebug(),
                    "[SERVER] Client " + std::to_string(static_cast<int>
                    (std::get<0>(clientToAdd))) + " added to lobby: " + lobbyCode,
                    debug::debugType::NETWORK, debug::debugLevel::INFO);
                break;
            }
        }
    } else {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Lobby code not found: " + lobbyCode,
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }
    return true;
}

bool rserv::Server::processMasterStart(std::pair<asio::ip::udp::endpoint,
    std::vector<uint8_t>> payload) {
    /* Verify Network */
    if (!this->_network) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Warning: Network not initialized",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }
    /* Verify that client is lobby master */
    std::string lobbyCode = "";
    if (payload.second.size() > HEADER_SIZE)
        lobbyCode = std::string(payload.second.begin() + HEADER_SIZE, payload.second.end());
    bool lobbyExists = false;
    for (const auto &lobby : this->_lobbyThreads) {
        if (lobby->_lobbyCode == lobbyCode) {
            lobbyExists = true;
            break;
        }
    }
    if (!lobbyExists) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Lobby code not found for master start: " + lobbyCode,
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }

    debug::Debug::printDebug(this->_config->getIsDebug(),
        "[SERVER] Lobby master requested game start for lobby: " + lobbyCode,
        debug::debugType::NETWORK, debug::debugLevel::INFO);

    for (const auto &lobby : this->_lobbyThreads) {
        if (lobby->_lobbyCode == lobbyCode) {
            for (const auto &lobbyEndpoint : lobby->_clients) {
                for (const auto &client : this->_clients) {
                    if (std::get<1>(client) == std::get<1>(lobbyEndpoint)) {
                        uint8_t clientId = std::get<0>(client);
                        this->_clientsReady[clientId] = true;
                        debug::Debug::printDebug(this->_config->getIsDebug(),
                            "[SERVER] Set client " + std::to_string(static_cast<int>
                            (clientId)) + " ready for lobby start",
                            debug::debugType::NETWORK, debug::debugLevel::INFO);
                    }
                }
            }
            break;
        }
    }

    std::vector<std::tuple<uint8_t, asio::ip::udp::endpoint, std::string>> _clientInfo;
    std::vector<asio::ip::udp::endpoint> endpoints;
    for (const auto &lobby : this->_lobbyThreads) {
        if (lobby->_lobbyCode == lobbyCode) {
            _clientInfo = lobby->_clients;
            break;
        }
    }
    for (const auto &client : _clientInfo) {
        endpoints.push_back(std::get<1>(client));
    }
    this->_lobbies.push_back(std::make_shared<Lobby>(
        this->_network,
        _clientInfo,
        lobbyCode,
        this->_config->getIsDebug()
    ));
    auto lobby = this->_lobbies.back();
    if (lobby == nullptr)
        std::cerr << "Lobby is null" << std::endl;
    for (const auto& client : _clientInfo) {
        uint8_t clientId = std::get<0>(client);
        this->_clientToLobby[clientId] = lobby;
    }
    lobby->setPacketManager(this->createNewPacketManager());
    this->initRessourceManager(lobby);
    this->canStartPacket(endpoints);
    lobby->startNetworkThread();
    lobby->startGameThread();
    debug::Debug::printDebug(this->_config->getIsDebug(),
        "[SERVER] Started lobby threads for lobby: " + lobbyCode,
        debug::debugType::NETWORK, debug::debugLevel::INFO);
    return true;
}
