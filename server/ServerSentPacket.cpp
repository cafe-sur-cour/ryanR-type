/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ServerSentPacket
*/

#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <utility>
#include <thread>
#include <chrono>
#include <nlohmann/json.hpp>
#include <algorithm>
#include "Server.hpp"
#include "Constants.hpp"
#include "Utils.hpp"
#include "../common/debug.hpp"
#include "../common/translationToECS.hpp"
#include "../common/ECS/entity/Entity.hpp"
#include "../common/ECS/entity/registry/Registry.hpp"
#include "../common/Parser/Parser.hpp"
#include "../common/utils/SecureJsonManager.hpp"

bool rserv::Server::connectionPacket(const net::INetworkEndpoint& endpoint) {
    std::vector<uint8_t> packet = this->_packet->pack(constants::ID_SERVER,
        this->_sequenceNumber, constants::PACKET_ACCEPT, std::vector<uint64_t>{
        static_cast<uint64_t>(this->_nextClientId)});

    if (!this->_network->sendTo(endpoint, packet)) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER NETWORK] Failed to send connection acceptance header to "
            + endpoint.getAddress() + ":" + std::to_string(endpoint.getPort()),
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return false;
    }
    this->_sequenceNumber++;
    return true;
}

bool rserv::Server::canStartPacket(std::vector<std::shared_ptr<net::INetworkEndpoint>>
    endpoints) {
    debug::Debug::printDebug(this->_config->getIsDebug(),
        "[SERVER] Sending canStart packet to " + std::to_string(endpoints.size()) + " clients",
        debug::debugType::NETWORK, debug::debugLevel::INFO);

    if (endpoints.empty()) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] No endpoints provided for canStart packet",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }

    std::vector<uint64_t> payload;
    for (const auto& endpoint : endpoints) {
        for (const auto& client : this->_clients) {
            if (std::get<1>(client) == endpoint) {
                std::string clientName = std::get<2>(client);
                std::vector<uint64_t> name = this->_packet->formatString(clientName);
                payload.insert(payload.end(), name.begin(), name.end());
                break;
            }
        }
    }

    std::vector<uint8_t> packetStart = this->_packet->pack(
        constants::ID_SERVER,
        this->_sequenceNumber,
        constants::PACKET_CAN_START,
        payload
    );

    if (!this->_network->broadcast(endpoints, packetStart)) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER NETWORK] Failed to broadcast can start packet to lobby clients",
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return false;
    }

    debug::Debug::printDebug(this->_config->getIsDebug(),
        "[SERVER] Successfully sent canStart packet to lobby clients",
        debug::debugType::NETWORK, debug::debugLevel::INFO);

    this->_sequenceNumber++;
    return true;
}

bool rserv::Server::serverStatusPacket() {
    size_t connectedClients = this->_clients.size();
    size_t readyClients = 0;

    for (const auto &client : this->_clients) {
        uint8_t clientId = std::get<0>(client);
        auto it = this->_clientsReady.find(clientId);
        if (it != this->_clientsReady.end() && it->second) {
            readyClients++;
        }
    }

    for (const auto &client : this->_clients) {
        uint8_t clientId = std::get<0>(client);
        bool isClientReady = false;
        auto it = this->_clientsReady.find(clientId);
        if (it != this->_clientsReady.end() && it->second) {
            isClientReady = true;
        }

        std::vector<uint64_t> payload = {
            static_cast<uint64_t>(connectedClients),
            static_cast<uint64_t>(readyClients),
            static_cast<uint64_t>(clientId),
            static_cast<uint64_t>(isClientReady ? 1 : 0)
        };

        std::vector<uint8_t> packet = this->_packet->pack(
            constants::ID_SERVER,
            this->_sequenceNumber,
            constants::PACKET_SERVER_STATUS,
            payload
        );

        if (!this->_network->sendTo(*std::get<1>(client), packet)) {
            debug::Debug::printDebug(this->_config->getIsDebug(),
                "[SERVER NETWORK] Failed to send server status packet to client " +
                std::to_string(static_cast<int>(clientId)),
                debug::debugType::NETWORK, debug::debugLevel::ERROR);
            return false;
        }
    }

    this->_sequenceNumber++;
    return true;
}

bool rserv::Server::sendCodeLobbyPacket(const net::INetworkEndpoint &endpoint) {
    /* Create random code */
    std::string lobbyCode;
    bool isUnique = false;
    int maxAttempts = 20;
    int attempts = 0;

    while (!isUnique && attempts < maxAttempts) {
        lobbyCode = Utils::createAlphaNumericCode();
        isUnique = true;
        for (const auto& lobby : this->_lobbyThreads) {
            if (lobby->_lobbyCode == lobbyCode) {
                isUnique = false;
                break;
            }
        }
        attempts++;
    }

    if (!isUnique) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Failed to generate unique lobby code after " +
            std::to_string(maxAttempts) + " attempts",
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return false;
    }
    /* Pack */
    std::vector<uint64_t> payload = this->_packet->formatString(lobbyCode);
    std::vector<uint8_t> packet = this->_packet->pack(
        constants::ID_SERVER,
        this->_sequenceNumber,
        constants::PACKET_SEND_LOBBY_CODE,
        payload
    );

    debug::Debug::printDebug(this->_config->getIsDebug(),
        "[SERVER] Generated unique lobby code: " + lobbyCode +
        " (attempt " + std::to_string(attempts) + ")",
        debug::debugType::NETWORK, debug::debugLevel::INFO);

    /* Send to requested client*/
    if (!this->_network->sendTo(endpoint, packet)) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER NETWORK] Failed to send lobby code packet to " +
            endpoint.getAddress() + ":" + std::to_string(endpoint.getPort()),
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return false;
    }
    /* Add to lobby vector, code and client endpoint */
    auto newLobby = std::make_shared<LobbyStruct>();
    newLobby->_lobbyCode = lobbyCode;
    this->_lobbyThreads.push_back(newLobby);
    this->_sequenceNumber++;
    return true;
}

bool rserv::Server::lobbyConnectValuePacket(const net::INetworkEndpoint &endpoint,
    bool isSucess) {
    if (!this->_network) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Warning: Network not initialized",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }
    std::vector<uint64_t> payload;
    payload.push_back(static_cast<uint64_t>(isSucess ? 't' : 'f'));
    std::vector<uint8_t> packet = this->_packet->pack(constants::ID_SERVER,
        this->_sequenceNumber, constants::PACKET_LOBBY_CONNECT_VALUE, payload);
    if (!this->_network->sendTo(endpoint, packet)) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Failed to send LOBBY_CONNECT_VALUE response to client",
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return false;
    }
    this->_sequenceNumber++;
    debug::Debug::printDebug(this->_config->getIsDebug(),
        "[SERVER] Sent LOBBY_CONNECT_VALUE response to client: " +
        std::string(isSucess ? "success" : "failure"),
        debug::debugType::NETWORK, (isSucess ? debug::debugLevel::INFO :
            debug::debugLevel::WARNING));
    return true;
}

bool rserv::Server::connectUserPacket(const net::INetworkEndpoint &endpoint,
    const std::string &username) {
    if (!this->_network) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Warning: Network not initialized",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }

    std::vector<uint64_t> payload;
    std::vector<uint64_t> nameData = this->_packet->formatString(username);
    payload.insert(payload.end(), nameData.begin(), nameData.end());

    std::vector<uint8_t> packet = this->_packet->pack(constants::ID_SERVER,
        this->_sequenceNumber, constants::PACKET_CONNECT_USER, payload);
    if (!this->_network->sendTo(endpoint, packet)) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Failed to send CONNECT_USER response to client",
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return false;
    }
    debug::Debug::printDebug(this->_config->getIsDebug(),
        "[SERVER] Sent CONNECT_USER packet to client with username: " + username,
        debug::debugType::NETWORK, debug::debugLevel::INFO);
    this->_sequenceNumber++;
    return true;
}

bool rserv::Server::leaderboardPacket(const net::INetworkEndpoint &endpoint) {
    if (!this->_network) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Warning: Network not initialized",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }

    const std::string filepath = constants::SCORES_JSON_PATH;
    nlohmann::json scores = utils::SecureJsonManager::readSecureJson(filepath);

    if (!scores.is_object()) {
        scores = nlohmann::json::object();
    }

    std::vector<std::pair<std::string, int>> leaderboard;
    for (const auto& [username, userData] : scores.items()) {
        if (userData.is_object() && userData.contains("scores") && userData["scores"].is_array()) {
            const auto& scoreArray = userData["scores"];
            for (const auto& score : scoreArray) {
                if (score.is_number()) {
                    leaderboard.emplace_back(username, score.get<int>());
                }
            }
        }
    }

    std::sort(leaderboard.begin(), leaderboard.end(),
        [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
            return a.second > b.second;
        });

    if (leaderboard.size() > 10) {
        leaderboard.resize(10);
    }

    std::vector<uint64_t> payload;
    for (const auto& entry : leaderboard) {
        std::vector<uint64_t> nameData = this->_packet->formatString(entry.first);
        payload.insert(payload.end(), nameData.begin(), nameData.end());
        std::vector<uint64_t> scoreData = this->_packet->formatString(std::to_string(entry.second));
        payload.insert(payload.end(), scoreData.begin(), scoreData.end());
    }

    std::vector<uint8_t> packet = this->_packet->pack(constants::ID_SERVER,
        this->_sequenceNumber, constants::PACKET_LEADERBOARD, payload);
    if (!this->_network->sendTo(endpoint, packet)) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Failed to send LEADERBOARD response to client",
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return false;
    }
    debug::Debug::printDebug(this->_config->getIsDebug(),
        "[SERVER] Sent LEADERBOARD packet to client with " + std::to_string(leaderboard.size()) + " entries",
        debug::debugType::NETWORK, debug::debugLevel::INFO);
    this->_sequenceNumber++;
    return true;
}
