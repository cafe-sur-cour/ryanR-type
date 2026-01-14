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
#include <fstream>
#include <map>
#include <set>
#include <sstream>
#include <algorithm>

#include "Server.hpp"
#include "constants.hpp"
#include "packet/DefaultPacketHandlers.hpp"
#include "../libs/Network/Unix/ServerNetwork.hpp"
#include "../../common/systems/systemManager/SystemManager.hpp"
#include "initResourcesManager/initResourcesManager.hpp"
#include "../common/Error/ServerErrror.hpp"
#include "../common/debug.hpp"
#include "../common/constants.hpp"
#include "../common/interfaces/IPacketManager.hpp"
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
    this->_clientLastHeartbeat = {};
    this->_config = std::make_shared<rserv::ServerConfig>();
    this->_httpServer = std::make_unique<rserv::HttpServer>(
        [this]() { return this->getState() == 1; },
        [this]() { return this->getServerInfo(); },
        this->_config,
        [this](const std::string& command) { return this->executeCommand(command); }
    );

    this->_httpServer->start();
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

        this->checkClientTimeouts();

        this->cleanupClosedLobbies();

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
    this->_httpServer->stop();
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

void rserv::Server::checkClientTimeouts() {
    auto now = std::chrono::steady_clock::now();
    std::vector<uint8_t> timedOutClients;

    {
        std::lock_guard<std::mutex> lock(_clientsMutex);
        for (const auto& client : this->_clients) {
            uint8_t clientId = std::get<0>(client);
            auto it = _clientLastHeartbeat.find(clientId);

            if (it != _clientLastHeartbeat.end()) {
                auto timeSinceLastHeartbeat =
                std::chrono::duration_cast<std::chrono::seconds>(
                    now - it->second).count();

                if (timeSinceLastHeartbeat > constants::CLIENT_TIMEOUT_SECONDS) {
                    debug::Debug::printDebug(this->_config->getIsDebug(),
                        "Client " + std::to_string(clientId) +
                            " timed out (no healthcheck for "
                            + std::to_string(timeSinceLastHeartbeat) + "s)",
                        debug::debugType::NETWORK, debug::debugLevel::WARNING);
                    timedOutClients.push_back(clientId);
                }
            }
        }
    }

    for (uint8_t clientId : timedOutClients) {
        this->_clientLastHeartbeat.erase(clientId);
        this->processDisconnections(clientId);
    }
}

void rserv::Server::cleanupClosedLobbies() {
    bool _hasUpdate = false;
    std::vector<std::shared_ptr<Lobby>> activeLobbies;
    std::vector<std::shared_ptr<LobbyStruct>> activeLobbyThreads;
    std::set<std::string> removedLobbyCodes;

    for (const auto& lobby : this->_lobbies) {
        if (lobby && !lobby->isRunning()) {
            _hasUpdate = true;
            removedLobbyCodes.insert(lobby->getLobbyCode());
            std::cout << "[SERVER] Removing stopped lobby: " << lobby->getLobbyCode() << std::endl;
            debug::Debug::printDebug(this->_config->getIsDebug(),
                "[SERVER] Removing stopped lobby: " + lobby->getLobbyCode(),
                debug::debugType::NETWORK, debug::debugLevel::INFO);
        } else {
            activeLobbies.push_back(lobby);
        }
    }

    for (const auto& lobbyStruct : this->_lobbyThreads) {
        if (lobbyStruct && removedLobbyCodes.find(lobbyStruct->_lobbyCode) == removedLobbyCodes.end()) {
            activeLobbyThreads.push_back(lobbyStruct);
        } else if (lobbyStruct && removedLobbyCodes.find(lobbyStruct->_lobbyCode) != removedLobbyCodes.end()) {
            debug::Debug::printDebug(this->_config->getIsDebug(),
                "[SERVER] Removing lobby thread: " + lobbyStruct->_lobbyCode,
                debug::debugType::NETWORK, debug::debugLevel::INFO);
        }
    }

    if (_hasUpdate) {
        this->_lobbies = activeLobbies;
        this->_lobbyThreads = activeLobbyThreads;
        std::cout << "Size after cleanup: " << this->_lobbies.size() << " lobbies remaining" << std::endl;
        std::cout << "Size after cleanup: " << this->_lobbyThreads.size() << " lobby threads remaining." << std::endl;
        for (const auto& lobbyStruct : this->_lobbyThreads) {
            if (lobbyStruct) {
                std::cout << "Remaining lobby code: " << lobbyStruct->_lobbyCode << std::endl;
            }
        }
    }
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
        this->requestCode(*received.first);
    } else if (this->_packet->getType() == constants::PACKET_CONNECT_TO_LOBBY) {
        this->processConnectToLobby(std::make_pair(received.first, received.second));
    } else if (this->_packet->getType() == constants::PACKET_LOBBY_MASTER_REQUEST_START) {
        this->processMasterStart(std::make_pair(received.first, received.second));
    } else if (this->_packet->getType() == constants::PACKET_EVENT) {
        uint8_t idClient = this->_packet->getIdClient();
        auto payload = this->_packet->getPayload();
        constants::EventType eventType = constants::EventType::UP;
        if (payload.size() >= 1) {
            eventType = static_cast<constants::EventType>(payload.at(0));
            if (eventType == constants::EventType::HEALTHCHECK) {
                this->_clientLastHeartbeat[idClient] = std::chrono::steady_clock::now();
            }
        }
        auto it = this->_clientToLobby.find(idClient);
        if (it != this->_clientToLobby.end()) {
            it->second->enqueuePacket(received);
        } else if (eventType != constants::EventType::HEALTHCHECK) {
            debug::Debug::printDebug(this->_config->getIsDebug(),
                "[SERVER] Received event packet from unknown client: " +
                std::to_string(idClient),
                debug::debugType::NETWORK, debug::debugLevel::WARNING);
        }
    } else if (this->_packet->getType() == constants::PACKET_REGISTER) {
        this->processRegistration(std::make_pair(received.first, received.second));
    } else if (this->_packet->getType() == constants::PACKET_LOGIN) {
        this->processLogin(std::make_pair(received.first, received.second));
    } else if (this->_packet->getType() == constants::PACKET_REQUEST_LEADERBOARD) {
        this->processLeaderboardRequest(received.first);
    } else if (this->_packet->getType() == constants::PACKET_REQUEST_PROFILE) {
        this->processProfileRequest(received.first);
    } else if (this->_packet->getType() == constants::PACKET_NEW_CHAT) {
        this->processNewChatMessage(std::make_pair(received.first, received.second));
    } else if (this->_packet->getType() == constants::PACKET_REQUEST_GAME_RULES_UPDATE) {
        this->processRequestGameRulesUpdate(std::make_pair(received.first, received.second));
    } else if (this->_packet->getType() == constants::PACKET_DISC) {
        uint8_t idClient = this->_packet->getIdClient();
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Received disconnection packet from client " + std::to_string(idClient),
            debug::debugType::NETWORK, debug::debugLevel::INFO);
        this->processDisconnections(idClient);
    } else if (this->_packet->getType() == constants::PACKET_LEAVE_LOBBY) {
        uint8_t idClient = this->_packet->getIdClient();
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Received leave lobby packet from client " + std::to_string(idClient),
            debug::debugType::NETWORK, debug::debugLevel::INFO);

        auto it = this->_clientToLobby.find(idClient);
        if (it != this->_clientToLobby.end() && it->second) {
            std::string lobbyCode = it->second->getLobbyCode();
            auto lobby = it->second;
            if (!lobby->isRunning()) {
                debug::Debug::printDebug(this->_config->getIsDebug(),
                    "[SERVER] Lobby " + lobbyCode + " is no longer running, skipping disconnect processing",
                    debug::debugType::NETWORK, debug::debugLevel::WARNING);
                this->_clientToLobby.erase(idClient);
                return;
            }
            for (auto& lobbyStruct : this->_lobbyThreads) {
                if (lobbyStruct && lobbyStruct->_lobbyCode == lobbyCode) {
                    auto& clients = lobbyStruct->_clients;
                    clients.erase(
                        std::remove_if(clients.begin(), clients.end(),
                            [idClient](const auto& client) {
                                return std::get<0>(client) == idClient;
                            }),
                        clients.end()
                    );
                    break;
                }
            }
            lobby->processDisconnections(idClient);
            this->_clientToLobby.erase(idClient);
            if (lobby->getClientCount() == 0 && lobby->isRunning()) {
                lobby->stop();
                debug::Debug::printDebug(this->_config->getIsDebug(),
                    "[SERVER] Lobby " + lobbyCode + " is empty and has been stopped",
                    debug::debugType::NETWORK, debug::debugLevel::INFO);
            }

        } else {
            debug::Debug::printDebug(this->_config->getIsDebug(),
                "[SERVER] Client " + std::to_string(idClient) + " tried to leave lobby but is not in any lobby",
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
    this->_clientLastHeartbeat[idClient] = std::chrono::steady_clock::now();
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
        throw err::ServerError("[Server] Cannot get createPacketInstance symbol",
            err::ServerError::LIBRARY_LOAD_FAILED);
    }
    auto packet = std::shared_ptr<pm::IPacketManager>
        (reinterpret_cast<pm::IPacketManager *>(createPacket()));
    if (!packet) {
        throw err::ServerError("[Server] Creating packet instance failed",
            err::ServerError::LIBRARY_LOAD_FAILED);
    }
    if (!rserv::packet::registerDefaultPacketHandlers(packet)) {
        throw err::ServerError("[Server] Registering default packet handlers failed",
            err::ServerError::LIBRARY_LOAD_FAILED);
    }
    return packet;
}

uint32_t rserv::Server::getNextEntityId() {
    return this->_nextEntityId++;
}

std::map<std::string, int> rserv::Server::loadUserStats(const std::string& username) const {
    std::map<std::string, int> stats = {
        {constants::GAMES_PLAYED_JSON_WARD, 0},
        {constants::WINS_JSON_WARD, 0},
        {constants::HIGH_SCORE_JSON_WARD, 0},
        {constants::BANNED_JSON_WARD, 0}
    };

    const std::string filepath = constants::USERS_JSON_PATH;
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return stats;
    }

    nlohmann::json users;
    try {
        file >> users;
        file.close();
    } catch (const std::exception&) {
        return stats;
    }

    if (!users.is_array()) {
        return stats;
    }

    for (const auto& user : users) {
        if (user.is_object() && user.contains(constants::USERNAME_JSON_WARD) &&
            user[constants::USERNAME_JSON_WARD].is_string() &&
                user[constants::USERNAME_JSON_WARD] == username) {
            if (user.contains(constants::GAMES_PLAYED_JSON_WARD) &&
                user[constants::GAMES_PLAYED_JSON_WARD].is_number_integer()) {
                stats[constants::GAMES_PLAYED_JSON_WARD] =
                    user[constants::GAMES_PLAYED_JSON_WARD];
            }
            if (user.contains(constants::WINS_JSON_WARD) &&
                user[constants::WINS_JSON_WARD].is_number_integer()) {
                stats[constants::WINS_JSON_WARD] = user[constants::WINS_JSON_WARD];
            }
            if (user.contains(constants::HIGH_SCORE_JSON_WARD) &&
                user[constants::HIGH_SCORE_JSON_WARD].is_number_integer()) {
                stats[constants::HIGH_SCORE_JSON_WARD] =
                    user[constants::HIGH_SCORE_JSON_WARD];
            }
            if (user.contains(constants::BANNED_JSON_WARD) &&
                user[constants::BANNED_JSON_WARD].is_boolean()) {
                stats[constants::BANNED_JSON_WARD] =
                    user[constants::BANNED_JSON_WARD] ? 1 : 0;
            }
            break;
        }
    }

    return stats;
}

void rserv::Server::saveUserBannedStatus(const std::string& username, bool banned) const {
    const std::string filepath = constants::USERS_JSON_PATH;
    std::ifstream infile(filepath);
    nlohmann::json users;

    if (infile.is_open()) {
        try {
            infile >> users;
            infile.close();
        } catch (const std::exception&) {
            users = nlohmann::json::array();
        }
    } else {
        users = nlohmann::json::array();
    }

    if (!users.is_array()) {
        users = nlohmann::json::array();
    }

    bool userFound = false;
    for (auto& user : users) {
        if (user.is_object() && user.contains(constants::USERNAME_JSON_WARD) &&
            user[constants::USERNAME_JSON_WARD].is_string() &&
            user[constants::USERNAME_JSON_WARD] == username) {
            user[constants::BANNED_JSON_WARD] = banned;
            userFound = true;
            break;
        }
    }

    if (!userFound) {
        nlohmann::json newUser;
        newUser[constants::USERNAME_JSON_WARD] = username;
        newUser[constants::BANNED_JSON_WARD] = banned;
        newUser[constants::GAMES_PLAYED_JSON_WARD] = 0;
        newUser[constants::WINS_JSON_WARD] = 0;
        newUser[constants::HIGH_SCORE_JSON_WARD] = 0;
        newUser[constants::TIME_SPENT_JSON_WARD] = 0;
        users.push_back(newUser);
    }

    std::ofstream outfile(filepath);
    if (outfile.is_open()) {
        outfile << users.dump(4);
        outfile.close();
    }
}

rserv::ServerInfo rserv::Server::getServerInfo() const {
    ServerInfo info;
    auto now = std::chrono::steady_clock::now();

    info.connectedClients = static_cast<int>(this->_clients.size());

    static auto startTime = std::chrono::steady_clock::now();
    info.uptime = std::chrono::duration_cast<std::chrono::seconds>(now - startTime);

    info.activeLobbies = static_cast<int>(this->_lobbies.size());

    info.totalPlayers = 0;
    for (const auto& lobbyPtr : this->_lobbies) {
        if (lobbyPtr) {
            info.totalPlayers += lobbyPtr->getClientCount();
        }
    }

    for (size_t i = 0; i < this->_lobbies.size(); ++i) {
        if (this->_lobbies[i]) {
            std::string lobbyInfo = "Lobby " +
                std::to_string(i + 1) + " (" + this->_lobbies[i]->getLobbyCode() + "): " +
                this->_lobbies[i]->getGameState() +
                " | " + this->_lobbies[i]->getGameRules();
            info.lobbyDetails.push_back(lobbyInfo);

            std::vector<std::string> lobbyPlayers;
            auto clientDetails = this->_lobbies[i]->getConnectedClientDetails();
            for (const auto& clientDetail : clientDetails) {
                std::string playerInfo = "Player ID: " +
                    std::to_string(std::get<0>(clientDetail)) +
                    ", Username: " + std::get<1>(clientDetail);
                lobbyPlayers.push_back(playerInfo);
            }
            info.lobbyPlayerDetails.push_back(lobbyPlayers);
        }
    }

    for (const auto& client : this->_clients) {
        std::string username = std::get<2>(client);
        if (!username.empty()) {
            std::string playerInfo = "Player ID: " + std::to_string(std::get<0>(client)) +
                ", Username: " + username;
            info.playerDetails.push_back(playerInfo);

            auto stats = this->loadUserStats(username);
            info.playerStats.push_back(stats);
        }
    }

    std::set<std::string> inGameUsernames;
    for (const auto& lobbyPtr : this->_lobbies) {
        if (lobbyPtr) {
            std::string gameState = lobbyPtr->getGameState();
            if (gameState.find("Classic mode") != std::string::npos ||
                gameState.find("Infinite Mode") != std::string::npos) {
                auto clientDetails = lobbyPtr->getConnectedClientDetails();
                for (const auto& clientDetail : clientDetails) {
                    std::string username = std::get<1>(clientDetail);
                    if (!username.empty()) {
                        inGameUsernames.insert(username);
                    }
                }
            }
        }
    }

    for (const auto& client : this->_clients) {
        std::string username = std::get<2>(client);
        if (!username.empty() && inGameUsernames.count(username) > 0) {
            std::string playerInfo = "Player ID: " + std::to_string(std::get<0>(client)) +
                ", Username: " + username;
            info.inGamePlayers.push_back(playerInfo);
        }
    }

    const std::string filepath = constants::USERS_JSON_PATH;
    std::ifstream file(filepath);
    if (file.is_open()) {
        nlohmann::json users;
        try {
            file >> users;
            file.close();
        } catch (const std::exception&) {
        }

        if (users.is_array()) {
            for (const auto& user : users) {
                if (user.is_object() && user.contains(constants::USERNAME_JSON_WARD) &&
                    user[constants::USERNAME_JSON_WARD].is_string()) {
                    std::string username = user[constants::USERNAME_JSON_WARD];
                    bool isBanned = user.contains(constants::BANNED_JSON_WARD) &&
                        user[constants::BANNED_JSON_WARD].is_boolean() &&
                        user[constants::BANNED_JSON_WARD].get<bool>();

                    if (isBanned && !username.empty()) {
                        uint8_t playerId = 0;
                        for (const auto& client : this->_clients) {
                            if (std::get<2>(client) == username) {
                                playerId = std::get<0>(client);
                                break;
                            }
                        }

                        std::string playerInfo;
                        if (playerId != 0) {
                            playerInfo = "Player ID: " + std::to_string(playerId) +
                                ", Username: " + username;
                        } else {
                            playerInfo = "Username: " + username + " (Offline)";
                        }
                        info.bannedPlayers.push_back(playerInfo);
                    }
                }
            }
        }
    }

    return info;
}

std::string rserv::Server::executeCommand(const std::string& command) {
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;

    if (cmd == "/close") {
        std::string lobbyId;
        iss >> lobbyId;
        return closeLobby(lobbyId);
    } else if (cmd == "/kick") {
        std::string playerId;
        iss >> playerId;
        return kickPlayer(playerId);
    } else if (cmd == "/ban") {
        std::string playerId;
        iss >> playerId;
        return banPlayer(playerId);
    } else if (cmd == "/unban") {
        std::string playerId;
        iss >> playerId;
        return unbanPlayer(playerId);
    } else {
        return "Unknown command";
    }
}

std::string rserv::Server::closeLobby(const std::string& lobbyId) {
    try {
        size_t id = std::stoul(lobbyId);
        if (id >= 1 && id <= _lobbies.size()) {
            size_t index = id - 1;
            if (_lobbies[index]) {
                auto lobbyPtr = _lobbies[index];
                auto connectedClients = lobbyPtr->getConnectedClients();
                for (auto clientId : connectedClients) {
                    lobbyPtr->removeClient(clientId);
                    for (auto& client : _clients) {
                        if (std::get<0>(client) == clientId) {
                            forceLeavePacket(
                                *std::get<1>(client), constants::ForceLeaveType::CLOSED);
                            break;
                        }
                    }
                }
                return "Lobby " + lobbyId + " closed";
            }
        }
    } catch (const std::exception&) {
    }

    for (auto& lobbyPtr : _lobbies) {
        if (lobbyPtr && lobbyPtr->getLobbyCode() == lobbyId) {
            auto connectedClients = lobbyPtr->getConnectedClients();
            for (auto clientId : connectedClients) {
                lobbyPtr->removeClient(clientId);
                for (auto& client : _clients) {
                    if (std::get<0>(client) == clientId) {
                        forceLeavePacket(
                            *std::get<1>(client), constants::ForceLeaveType::CLOSED);
                        break;
                    }
                }
            }
            return "Lobby " + lobbyId + " closed";
        }
    }
    return "Lobby not found";
}

std::string rserv::Server::kickPlayer(const std::string& playerId) {
    uint8_t id = 0;

    try {
        id = static_cast<uint8_t>(std::stoi(playerId));
    } catch (const std::exception&) {
        for (const auto& client : _clients) {
            std::string username = std::get<2>(client);
            if (username == playerId) {
                id = std::get<0>(client);
                break;
            }
        }
        if (id == 0) {
            return "Player not found";
        }
    }

    if (_clientToLobby.find(id) != _clientToLobby.end()) {
        auto lobby = _clientToLobby[id];
        lobby->removeClient(id);
        for (auto& client : _clients) {
            if (std::get<0>(client) == id) {
                forceLeavePacket(*std::get<1>(client), constants::ForceLeaveType::KICKED);
                return "Player " + playerId + " kicked";
            }
        }
    }
    return "Player not found";
}

std::string rserv::Server::banPlayer(const std::string& playerId) {
    uint8_t id = 0;
    std::string username;

    try {
        id = static_cast<uint8_t>(std::stoi(playerId));
        for (const auto& client : _clients) {
            if (std::get<0>(client) == id) {
                username = std::get<2>(client);
                break;
            }
        }
    } catch (const std::exception&) {
        username = playerId;
        for (const auto& client : _clients) {
            if (std::get<2>(client) == username) {
                id = std::get<0>(client);
                break;
            }
        }
    }

    if (username.empty()) {
        return "Player not found";
    }

    saveUserBannedStatus(username, true);

    if (id != 0 && _clientToLobby.find(id) != _clientToLobby.end()) {
        auto lobby = _clientToLobby[id];
        lobby->removeClient(id);
        for (auto& client : _clients) {
            if (std::get<0>(client) == id) {
                forceLeavePacket(*std::get<1>(client), constants::ForceLeaveType::BANNED);
                break;
            }
        }
    }

    return "Player " + playerId + " banned";
}

std::string rserv::Server::unbanPlayer(const std::string& playerId) {
    std::string username = playerId;

    try {
        uint8_t id = static_cast<uint8_t>(std::stoi(playerId));
        for (const auto& client : _clients) {
            if (std::get<0>(client) == id) {
                username = std::get<2>(client);
                break;
            }
        }
    } catch (const std::exception&) {
    }

    if (!username.empty()) {
        saveUserBannedStatus(username, false);
        return "Player " + playerId + " unbanned";
    }
    return "Player not found";
}

uint8_t rserv::Server::findClientIdByEndpoint(const net::INetworkEndpoint &endpoint) const {
    for (const auto& client : this->_clients) {
        if (*std::get<1>(client) == endpoint) {
            return std::get<0>(client);
        }
    }
    return 0;
}
