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
#include <utility>
#include <thread>
#include <tuple>
#include <fstream>
#include <filesystem>   // NOLINT(build/c++17)
#include <nlohmann/json.hpp>

#include "Server.hpp"
#include "Lobby.hpp"
#include "constants.hpp"
#include "../../../common/utils/SecureJsonManager.hpp"
#include "../common/utils/Encryption.hpp"
#include "../common/debug.hpp"
#include "../common/components/tags/PlayerTag.hpp"
#include "../common/ECS/entity/registry/Registry.hpp"
#include "../common/GameRules.hpp"

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
            if (this->_clientToLobby.find(idClient) != this->_clientToLobby.end()) {
                auto lobby = this->_clientToLobby[idClient];
                if (lobby) {
                    lobby->processDisconnections(idClient);
                }
                this->_clientToLobby.erase(idClient);
                debug::Debug::printDebug(this->_config->getIsDebug(),
                    "Client " + std::to_string(idClient) + " removed from lobby",
                    debug::debugType::NETWORK, debug::debugLevel::INFO);

                // Don't call cleanupClosedLobbies() immediately - let the normal cycle handle it
                // This prevents removing lobbies that the client might reconnect to quickly
            }

            this->_clients.erase(
                std::remove(this->_clients.begin(), this->_clients.end(), client),
                this->_clients.end());
            this->_nextClientId--;
            this->_clientLastHeartbeat.erase(idClient);
            debug::Debug::printDebug(this->_config->getIsDebug(),
                "Client " + std::to_string(idClient)
                + " disconnected and removed from server",
                debug::debugType::NETWORK, debug::debugLevel::INFO);
            return true;
        }
    }
    return false;
}

bool rserv::Server::requestCode(const net::INetworkEndpoint &endpoint) {
    std::string username;
    for (const auto& client : this->_clients) {
        if (*std::get<1>(client) == endpoint) {
            username = std::get<2>(client);
            break;
        }
    }

    if (!username.empty()) {
        auto stats = this->loadUserStats(username);
        if (stats[constants::BANNED_JSON_WARD] == 1) {
            debug::Debug::printDebug(this->_config->getIsDebug(),
                "[SERVER] Banned user " + username + " tried to request lobby code",
                debug::debugType::NETWORK, debug::debugLevel::WARNING);
            return false;
        }
    }

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

    if (lobbyCode == constants::LOBBY_LEAVE_MARKER ||
        lobbyCode.find(constants::LOBBY_LEAVE_KEYWORD) != std::string::npos) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Client attempting to leave lobby",
            debug::debugType::NETWORK, debug::debugLevel::INFO);

        uint8_t clientIdToRemove = 0;
        bool clientFound = false;
        for (const auto &client : this->_clients) {
            if (std::get<1>(client) && payload.first &&
                std::get<1>(client)->getAddress() == payload.first->getAddress() &&
                std::get<1>(client)->getPort() == payload.first->getPort()) {
                clientIdToRemove = std::get<0>(client);
                clientFound = true;
                break;
            }
        }

        if (!clientFound) {
            debug::Debug::printDebug(this->_config->getIsDebug(),
                "[SERVER] Error: Client not found in _clients list for lobby leave",
                debug::debugType::NETWORK, debug::debugLevel::ERROR);
            return false;
        }

        if (this->_clientToLobby.find(clientIdToRemove) != this->_clientToLobby.end()) {
            auto lobby = this->_clientToLobby[clientIdToRemove];
            if (lobby) {
                lobby->processDisconnections(clientIdToRemove);
                this->_clientToLobby.erase(clientIdToRemove);
                debug::Debug::printDebug(this->_config->getIsDebug(),
                    "[SERVER] Client " + std::to_string(static_cast<int>(clientIdToRemove)) +
                    " left the lobby",
                    debug::debugType::NETWORK, debug::debugLevel::INFO);
            }
        }
        this->lobbyConnectValuePacket(*payload.first, true);
        return true;
    }

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
                uint8_t clientId = std::get<0>(clientToAdd);

                bool alreadyInLobbyThread = false;
                for (const auto &existingClient : lobby->_clients) {
                    if (std::get<0>(existingClient) == clientId) {
                        alreadyInLobbyThread = true;
                        break;
                    }
                }

                if (!alreadyInLobbyThread) {
                    std::string username = std::get<2>(clientToAdd);
                    if (!username.empty()) {
                        auto userStats = this->loadUserStats(username);
                        if (userStats[constants::BANNED_JSON_WARD] == 1) {
                            debug::Debug::printDebug(this->_config->getIsDebug(),
                                "[SERVER] Banned user " + username +
                                    " attempted to join lobby",
                                debug::debugType::NETWORK, debug::debugLevel::WARNING);
                            this->lobbyConnectValuePacket(*payload.first, false);
                            return true;
                        }
                    }

                    lobby->_clients.push_back(clientToAdd);
                }

                for (auto &actualLobby : this->_lobbies) {
                    if (actualLobby->getLobbyCode() == lobbyCode) {
                        auto existingClients = actualLobby->getConnectedClients();
                        bool alreadyInLobby = false;
                        for (uint8_t existingId : existingClients) {
                            if (existingId == clientId) {
                                alreadyInLobby = true;
                                break;
                            }
                        }

                        if (!alreadyInLobby) {
                            actualLobby->addClient(clientToAdd);

                            actualLobby->syncExistingEntitiesToClient(
                                std::get<1>(clientToAdd));
                        }

                        this->_clientToLobby[clientId] = actualLobby;

                        actualLobby->gameRulesPacket();
                        break;
                    }
                }

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
        std::filesystem::create_directories(std::filesystem::path(filepath).parent_path());
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

    std::shared_ptr<Lobby> lobby = nullptr;
    for (auto &existingLobby : this->_lobbies) {
        if (existingLobby->getLobbyCode() == lobbyCode) {
            lobby = existingLobby;
            break;
        }
    }

    if (!lobby) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Error: Lobby not found in _lobbies for code: " + lobbyCode,
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return false;
    }

    this->canStartPacket(endpoints);
    lobby->gameRulesPacket();
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
    username.erase(username.find_last_not_of(constants::END_OFSTRING_TRD) + 1);

    std::string password = data.substr(8, 8);
    password.erase(password.find_last_not_of(constants::END_OFSTRING_TRD) + 1);

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

    std::string encryptedPassword = utils::Encryption::encrypt(password);

    nlohmann::json newUser;
    newUser[constants::USERNAME_JSON_WARD] = username;
    newUser[constants::PASSWORD_JSON_WARD] = encryptedPassword;
    newUser[constants::WINS_JSON_WARD] = 0;
    newUser[constants::HIGH_SCORE_JSON_WARD] = 0;
    newUser[constants::GAMES_PLAYED_JSON_WARD] = 0;
    newUser[constants::TIME_SPENT_JSON_WARD] = 0;
    users.push_back(newUser);
    std::filesystem::create_directories(std::filesystem::path(filepath).parent_path());
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
    username.erase(username.find_last_not_of(constants::END_OFSTRING_TRD) + 1);

    std::string password = data.substr(8, 8);
    password.erase(password.find_last_not_of(constants::END_OFSTRING_TRD) + 1);

    if (username.empty()) {
        for (auto &clientTuple : this->_clients) {
            if (std::get<1>(clientTuple) && client.first &&
                std::get<1>(clientTuple)->getAddress() == client.first->getAddress() &&
                std::get<1>(clientTuple)->getPort() == client.first->getPort()) {
                std::get<2>(clientTuple) = "";
                debug::Debug::printDebug(this->_config->getIsDebug(),
                    "[SERVER] User logged out",
                    debug::debugType::NETWORK, debug::debugLevel::INFO);
                break;
            }
        }
        return true;
    }

    if (password.empty()) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Warning: Empty password",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }

    bool alreadyLoggedIn = false;
    for (const auto& clientTuple : this->_clients) {
        if (std::get<2>(clientTuple) == username) {
            alreadyLoggedIn = true;
            break;
        }
    }
    if (alreadyLoggedIn) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Login failed: User already logged in",
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
            user[constants::USERNAME_JSON_WARD] == username) {
            std::string storedPassword =
                user[constants::PASSWORD_JSON_WARD].get<std::string>();
            std::string decryptedPassword = utils::Encryption::decrypt(storedPassword);
            if (decryptedPassword == password) {
                loginSuccess = true;
                break;
            }
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

bool rserv::Server::processNewChatMessage(std::pair<std::shared_ptr<net::INetworkEndpoint>,
    std::vector<uint8_t>> payload) {
    if (!this->_network) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Warning: Network not initialized",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }

    std::vector<uint8_t> message;
    message.insert(message.begin(), payload.second.begin() + HEADER_SIZE,
        payload.second.end());
    if (!this->newChatMessagePacket(*payload.first, message)) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Warning: Failed to send profile packet",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }
    return true;
}

bool rserv::Server::processRequestGameRulesUpdate(
    std::pair<std::shared_ptr<net::INetworkEndpoint>,
    std::vector<uint8_t>> payload) {
    if (!this->_network) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Warning: Network not initialized",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }

    this->_packet->unpack(payload.second);
    uint8_t clientId = this->_packet->getIdClient();
    auto packetPayload = this->_packet->getPayload();

    if (packetPayload.size() < 2) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Invalid GameRules update packet (size < 2)",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }

    uint8_t ruleType = static_cast<uint8_t>(packetPayload[0]);

    auto it = this->_clientToLobby.find(clientId);
    if (it == this->_clientToLobby.end()) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Client " + std::to_string(clientId) + " not found in any lobby",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }

    std::shared_ptr<Lobby> lobby = it->second;
    if (!lobby) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Lobby is null",
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return false;
    }

    auto resourceManager = lobby->getResourceManager();
    if (!resourceManager || !resourceManager->has<GameRules>()) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] GameRules not found in lobby",
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return false;
    }

    auto gameRules = resourceManager->get<GameRules>();

    /* ruleType: 0 = gamemode, 1 = difficulty, 2 = crossfire */
    if (ruleType == 0) {
        GameRulesNS::Gamemode current = gameRules->getGamemode();
        GameRulesNS::Gamemode next = GameRulesNS::Gamemode::CLASSIC;

        if (current == GameRulesNS::Gamemode::CLASSIC) next =
            GameRulesNS::Gamemode::INFINITE_MODE;
        else if (current == GameRulesNS::Gamemode::INFINITE_MODE) next =
            GameRulesNS::Gamemode::CLASSIC;

        gameRules->setGamemode(next);

        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] GameRules changed to gamemode: " +
            std::to_string(static_cast<int>(next)),
            debug::debugType::NETWORK, debug::debugLevel::INFO);
    } else if (ruleType == 1) {
        GameRulesNS::Difficulty current = gameRules->getDifficulty();
        GameRulesNS::Difficulty next = GameRulesNS::Difficulty::NORMAL;

        if (current == GameRulesNS::Difficulty::NORMAL) next =
            GameRulesNS::Difficulty::HARD;
        else if (current == GameRulesNS::Difficulty::HARD) next =
            GameRulesNS::Difficulty::EASY;
        else if (current == GameRulesNS::Difficulty::EASY) next =
            GameRulesNS::Difficulty::NORMAL;

        gameRules->setDifficulty(next);

        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] GameRules changed to difficulty: " +
            std::to_string(static_cast<int>(next)),
            debug::debugType::NETWORK, debug::debugLevel::INFO);
    } else if (ruleType == 2) {
        bool currentCrossfire = gameRules->getCrossfire();
        gameRules->setCrossfire(!currentCrossfire);

        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Crossfire toggled to: " +
            std::string(!currentCrossfire ? "ON" : "OFF"),
            debug::debugType::NETWORK, debug::debugLevel::INFO);
    } else {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Unknown GameRules update type: " + std::to_string(ruleType),
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }

    lobby->gameRulesPacket();

    return true;
}
