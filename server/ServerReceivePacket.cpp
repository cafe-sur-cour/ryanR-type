/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ServerReceivePacket
*/

#include <vector>
#include <iostream>
#include <string>

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
    this->canStartPacket();
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

bool rserv::Server::processEndOfGame(uint8_t idClient) {
    std::vector<uint8_t> packet = this->_packet->pack(constants::ID_SERVER,
        this->_sequenceNumber, constants::PACKET_END_GAME,
        std::vector<uint64_t>{static_cast<uint64_t>(idClient)});
    if (!this->_network->broadcast(this->getConnectedClientEndpoints(), packet)) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER NETWORK] Failed to broadcast end of game packet",
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return false;
    }
    return true;
}


bool rserv::Server::processEvents(uint8_t idClient) {
    debug::Debug::printDebug(this->_config->getIsDebug(),
        "[SERVER] Processing event packet from client: "
        + std::to_string(idClient),
        debug::debugType::NETWORK, debug::debugLevel::INFO);

    auto payload = this->_packet->getPayload();
    if (payload.size() < 2) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Invalid event packet payload size: " + std::to_string(payload.size()),
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return false;
    }

    constants::EventType eventType =
        static_cast<constants::EventType>(payload.at(0));

    uint64_t param1Bits = payload.at(1);
    double param1;
    std::memcpy(&param1, &param1Bits, sizeof(double));

    this->_eventQueue->push(std::tuple(idClient, eventType, param1));
    return true;
}

bool rserv::Server::processWhoAmI(uint8_t idClient) {
    debug::Debug::printDebug(this->_config->getIsDebug(),
        "[SERVER] Processing WHOAMI request from client: "
        + std::to_string(idClient),
        debug::debugType::NETWORK, debug::debugLevel::INFO);

    if (!this->_resourceManager->has<ecs::Registry>()) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Registry not found, cannot process WHOAMI",
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return false;
    }

    auto registry = this->_resourceManager->get<ecs::Registry>();
    auto playerView = registry->view<ecs::PlayerTag>();

    ecs::Entity playerEntity = 0;
    for (auto entityId : playerView) {
        if (entityId == idClient) {
            playerEntity = entityId;
            break;
        }
    }

    if (playerEntity == 0) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] No player entity found for client " + std::to_string(idClient),
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }

    std::vector<uint64_t> payload;
    payload.push_back(static_cast<uint64_t>(playerEntity));

    asio::ip::udp::endpoint clientEndpoint;
    for (const auto &client : this->_clients) {
        if (std::get<0>(client) == idClient) {
            clientEndpoint = std::get<1>(client);
            break;
        }
    }

    std::vector<uint8_t> packet = this->_packet->pack(constants::ID_SERVER,
        this->_sequenceNumber, constants::PACKET_WHOAMI, payload);

    if (!this->_network->sendTo(clientEndpoint, packet)) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Failed to send WHOAMI response to client " + std::to_string(idClient),
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return false;
    }

    this->_sequenceNumber++;
    debug::Debug::printDebug(this->_config->getIsDebug(),
        "[SERVER] Sent WHOAMI response to client " + std::to_string(idClient)
        + " with entity ID " + std::to_string(playerEntity),
        debug::debugType::NETWORK, debug::debugLevel::INFO);

    return true;
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
    for (const auto &lobby : this->_lobbys) {
        if (lobby.first == lobbyCode) {
            if (lobby.second.size() > constants::MAX_CLIENT_PER_LOBBY) {
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
    if (lobbyExists) {
        for (auto &lobby : this->_lobbys) {
            if (lobby.first == lobbyCode) {
                lobby.second.push_back(payload.first);
                debug::Debug::printDebug(this->_config->getIsDebug(),
                    "[SERVER] Client added to lobby: " + lobbyCode,
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
    for (const auto &lobby : this->_lobbys) {
        if (lobby.first == lobbyCode) {
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

    for (const auto &lobby : this->_lobbys) {
        if (lobby.first == lobbyCode) {
            for (const auto &lobbyEndpoint : lobby.second) {
                for (const auto &client : this->_clients) {
                    if (std::get<1>(client) == lobbyEndpoint) {
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
    this->canStartPacket();
    return true;
}
