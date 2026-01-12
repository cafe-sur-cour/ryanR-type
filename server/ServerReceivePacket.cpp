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
#include <tuple>
#include <fstream>
#include <nlohmann/json.hpp>

#include "Server.hpp"
#include "Constants.hpp"
#include "../../../common/utils/SecureJsonManager.hpp"
#include "../common/debug.hpp"
#include "../common/components/tags/PlayerTag.hpp"
#include "../common/ECS/entity/registry/Registry.hpp"

bool rserv::Server::processConnections(std::pair<std::shared_ptr<net::INetworkEndpoint>,
    std::vector<uint8_t>> client) {
    if (!_network) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Warning: Network not initialized",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }

    this->connectionPacket(*client.first);
    this->_clients.push_back(std::make_tuple(this->_nextClientId, client.first, ""));
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

bool rserv::Server::requestCode(const net::INetworkEndpoint &endpoint) {
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

bool rserv::Server::processConnectToLobby(std::pair<std::shared_ptr<net::INetworkEndpoint>,
    std::vector<uint8_t>> payload) {
    if (!this->_network) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Warning: Network not initialized",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }

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

    this->lobbyConnectValuePacket(*payload.first, lobbyExists);
    std::tuple<uint8_t, std::shared_ptr<net::INetworkEndpoint>, std::string> clientToAdd;
    bool clientFound = false;
    for (const auto &client : this->_clients) {
        if (std::get<1>(client) && payload.first &&
            std::get<1>(client)->getAddress() == payload.first->getAddress() &&
            std::get<1>(client)->getPort() == payload.first->getPort()) {
            clientToAdd = client;
            clientFound = true;
            break;
        }
    }
    if (!clientFound) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Error: Client not found in _clients list for lobby connection",
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return false;
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


bool rserv::Server::processMasterStart(std::pair<std::shared_ptr<net::INetworkEndpoint>,
    std::vector<uint8_t>> payload) {
    if (!this->_network) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Warning: Network not initialized",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }

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

    std::vector<std::tuple<uint8_t, std::shared_ptr<net::INetworkEndpoint>,
        std::string>> _clientInfo;
    std::vector<std::shared_ptr<net::INetworkEndpoint>> endpoints;
    for (const auto &lobby : this->_lobbyThreads) {
        if (lobby->_lobbyCode == lobbyCode) {
            _clientInfo = lobby->_clients;
            break;
        }
    }

    const std::string filepath = constants::USERS_JSON_PATH;
    nlohmann::json users;
    std::ifstream file(filepath);
    if (file.is_open()) {
        try {
            file >> users;
        } catch (const std::exception&) {
            users = nlohmann::json::array();
        }
        file.close();
    } else {
        users = nlohmann::json::array();
    }
    if (users.is_array()) {
        for (const auto& client : _clientInfo) {
            std::string username = std::get<2>(client);
            debug::Debug::printDebug(this->_config->getIsDebug(),
                "[SERVER] Incrementing games_played for user: " + username,
                debug::debugType::NETWORK, debug::debugLevel::INFO);
            for (auto& user : users) {
                if (user.is_object() && user.contains(constants::USERNAME_JSON_WARD) &&
                    user[constants::USERNAME_JSON_WARD].is_string() &&
                    user[constants::USERNAME_JSON_WARD] == username) {
                    int gamesPlayed = 0;
                    if (user.contains(constants::GAMES_PLAYED_JSON_WARD) &&
                        user[constants::GAMES_PLAYED_JSON_WARD].is_number_integer()) {
                        gamesPlayed = user[constants::GAMES_PLAYED_JSON_WARD].get<int>();
                    }
                    user[constants::GAMES_PLAYED_JSON_WARD] = gamesPlayed + 1;
                    debug::Debug::printDebug(this->_config->getIsDebug(),
                        "[SERVER] Incremented games_played to " + std::to_string(
                            gamesPlayed + 1) + " for " + username,
                        debug::debugType::NETWORK, debug::debugLevel::INFO);
                    break;
                }
            }
        }
        std::ofstream outfile(filepath);
        bool writeSuccess = false;
        if (outfile.is_open()) {
            outfile << users.dump(4);
            outfile.close();
            writeSuccess = true;
        }
        if (writeSuccess) {
            debug::Debug::printDebug(this->_config->getIsDebug(),
                "[SERVER] Successfully wrote updated users.json",
                debug::debugType::NETWORK, debug::debugLevel::INFO);
        } else {
            debug::Debug::printDebug(this->_config->getIsDebug(),
                "[SERVER] Failed to write users.json",
                debug::debugType::NETWORK, debug::debugLevel::ERROR);
        }
    } else {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] users.json is not an array",
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
    }

    for (const auto &client : _clientInfo) {
        endpoints.push_back(std::get<1>(client));
    }
    this->_lobbies.push_back(std::make_shared<Lobby>(
        this->_network,
        _clientInfo,
        lobbyCode,
        this->_config->getIsDebug(),
        this->_config->getTps()
    ));
    auto lobby = this->_lobbies.back();
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

bool rserv::Server::processRegistration(std::pair<std::shared_ptr<net::INetworkEndpoint>,
    std::vector<uint8_t>> client) {
    if (!_network) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Warning: Network not initialized",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }

    if (client.second.size() <= HEADER_SIZE) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Warning: Invalid registration packet size",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }

    std::string data(client.second.begin() + HEADER_SIZE, client.second.end());

    std::string username = data.substr(0, 8);
    username.erase(username.find_last_not_of('\0') + 1);

    std::string password = data.substr(8, 8);
    password.erase(password.find_last_not_of('\0') + 1);

    if (username.empty() || password.empty()) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Warning: Empty username or password",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }

    if (username.length() > 8) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Warning: Username too long",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }

    const std::string filepath = "saves/users.json";
    nlohmann::json users;
    std::ifstream file(filepath);
    if (file.is_open()) {
        try {
            file >> users;
        } catch (const std::exception&) {
            users = nlohmann::json::array();
        }
        file.close();
    } else {
        users = nlohmann::json::array();
    }

    if (!users.is_array()) {
        users = nlohmann::json::array();
    }

    for (const auto& user : users) {
        if (user.is_object() && user.contains(constants::USERNAME_JSON_WARD) &&
            user[constants::USERNAME_JSON_WARD] == username) {
            debug::Debug::printDebug(this->_config->getIsDebug(),
                "[SERVER] Registration failed: Username already exists",
                debug::debugType::NETWORK, debug::debugLevel::WARNING);

            std::vector<uint8_t> packet = this->_packet->pack(constants::ID_SERVER,
                this->_sequenceNumber++, constants::PACKET_REGISTER_FAIL, {});
            this->_network->sendTo(*client.first, packet);
            return false;
        }
    }

    nlohmann::json newUser;
    newUser[constants::USERNAME_JSON_WARD] = username;
    newUser[constants::PASSWORD_JSON_WARD] = password;
    newUser[constants::WINS_JSON_WARD] = 0;
    newUser[constants::HIGH_SCORE_JSON_WARD] = 0;
    newUser[constants::GAMES_PLAYED_JSON_WARD] = 0;
    newUser[constants::TIME_SPENT_JSON_WARD] = 0;
    users.push_back(newUser);
    std::ofstream outfile(filepath);
    bool writeSuccess = false;
    if (outfile.is_open()) {
        outfile << users.dump(4);
        outfile.close();
        writeSuccess = true;
    }
    if (writeSuccess) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] User registered successfully: " + username,
            debug::debugType::NETWORK, debug::debugLevel::INFO);

        for (auto &clientTuple : this->_clients) {
            if (std::get<1>(clientTuple) && client.first &&
                std::get<1>(clientTuple)->getAddress() == client.first->getAddress() &&
                std::get<1>(clientTuple)->getPort() == client.first->getPort()) {
                std::get<2>(clientTuple) = username;
                debug::Debug::printDebug(this->_config->getIsDebug(),
                    "[SERVER] Updated username for newly registered client: " + username,
                    debug::debugType::NETWORK, debug::debugLevel::INFO);
                break;
            }
        }

        if (!this->connectUserPacket(*client.first, username)) {
            debug::Debug::printDebug(this->_config->getIsDebug(),
                "[SERVER] Error: Failed to send CONNECT_USER packet",
                debug::debugType::NETWORK, debug::debugLevel::ERROR);
            return false;
        }
        return true;
    } else {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Error: Could not write users file",
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return false;
    }
    return true;
}

bool rserv::Server::processLogin(std::pair<std::shared_ptr<net::INetworkEndpoint>,
    std::vector<uint8_t>> client) {
    if (!_network) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Warning: Network not initialized",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }

    if (client.second.size() <= HEADER_SIZE) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Warning: Invalid login packet size",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }

    std::string data(client.second.begin() + HEADER_SIZE, client.second.end());

    std::string username = data.substr(0, 8);
    username.erase(username.find_last_not_of('\0') + 1);

    std::string password = data.substr(8, 8);
    password.erase(password.find_last_not_of('\0') + 1);

    if (username.empty() || password.empty()) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Warning: Empty username or password",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }

    const std::string filepath = constants::USERS_JSON_PATH;
    nlohmann::json users;
    std::ifstream file(filepath);
    if (file.is_open()) {
        try {
            file >> users;
        } catch (const std::exception&) {
            users = nlohmann::json::array();
        }
        file.close();
    } else {
        users = nlohmann::json::array();
    }

    if (!users.is_array()) {
        users = nlohmann::json::array();
    }

    bool loginSuccess = false;
    for (const auto& user : users) {
        if (user.is_object() && user.contains(constants::USERNAME_JSON_WARD) &&
            user.contains(constants::PASSWORD_JSON_WARD) &&
            user[constants::USERNAME_JSON_WARD] == username &&
            user[constants::PASSWORD_JSON_WARD] == password) {
            loginSuccess = true;
            break;
        }
    }

    if (loginSuccess) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] User logged in successfully: " + username,
            debug::debugType::NETWORK, debug::debugLevel::INFO);

        for (auto &clientTuple : this->_clients) {
            if (std::get<1>(clientTuple) && client.first &&
                std::get<1>(clientTuple)->getAddress() == client.first->getAddress() &&
                std::get<1>(clientTuple)->getPort() == client.first->getPort()) {
                std::get<2>(clientTuple) = username;
                debug::Debug::printDebug(this->_config->getIsDebug(),
                    "[SERVER] Updated username for client: " + username,
                    debug::debugType::NETWORK, debug::debugLevel::INFO);
                break;
            }
        }

        if (!this->connectUserPacket(*client.first, username)) {
            debug::Debug::printDebug(this->_config->getIsDebug(),
                "[SERVER] Error: Failed to send CONNECT_USER packet",
                debug::debugType::NETWORK, debug::debugLevel::ERROR);
            return false;
        }
        return true;
    } else {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Login failed for username: " + username,
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }
}

bool rserv::Server::processLeaderboardRequest(std::shared_ptr<net::INetworkEndpoint> client) {
    if (!this->_network) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Warning: Network not initialized",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }

    if (!this->leaderboardPacket(*client)) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Warning: Failed to send leaderboard packet",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }
    return true;
}

bool rserv::Server::processProfileRequest(std::shared_ptr<net::INetworkEndpoint> client) {
    if (!this->_network) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Warning: Network not initialized",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }

    if (!this->profilePacket(*client)) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Warning: Failed to send profile packet",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }
    return true;
}

bool rserv::Server::processNewChatMessage(std::pair<std::shared_ptr<net::INetworkEndpoint>, std::vector<uint8_t>> payload) {
    if (!this->_network) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Warning: Network not initialized",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }

    std::vector<uint8_t> message;
    message.insert(message.begin(), payload.second.begin() + HEADER_SIZE, payload.second.end());
    if (!this->newChatMessagePacket(*payload.first, message)) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Warning: Failed to send profile packet",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }
    return true;
}