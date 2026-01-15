/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ClientSentPacket
*/

#include <cstring>
#include <iostream>
#include <vector>
#include <string>

#include "ClientNetwork.hpp"
#include "../common/Error/ClientNetworkError.hpp"
#include "../common/debug.hpp"
#include "../common/constants.hpp"

/* Packet Handling */
void ClientNetwork::connectionPacket() {
    if (!_network) {
        throw err::ClientNetworkError("[ClientNetwork] Network not initialized",
            err::ClientNetworkError::INTERNAL_ERROR);
    }
    this->_retryCount = 0;
    std::vector<uint8_t> packet = this->_packet->pack(this->_idClient,
        this->_sequenceNumber, constants::PACKET_CONNECTION, {});

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Sending connection packet",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    this->_network->sendTo(*this->_serverEndpoint, packet);
    this->_sequenceNumber++;
}


void ClientNetwork::eventPacket(const constants::EventType &eventType,
    double depth) {
    if (!_network) {
        throw err::ClientNetworkError("[ClientNetwork] Network not initialized",
            err::ClientNetworkError::INTERNAL_ERROR);
    }

    uint64_t depthBits;
    std::memcpy(&depthBits, &depth, sizeof(double));

    std::vector<uint64_t> payload;
    payload.push_back(static_cast<uint64_t>(eventType));
    payload.push_back(depthBits);

    std::vector<uint8_t> packet =
        this->_packet->pack(this->_idClient, this->_sequenceNumber,
        constants::PACKET_EVENT, payload);

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Preparing to send event packet: EventType="
        + std::to_string(static_cast<int>(eventType))
        + ", Depth=" + std::to_string(depth),
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    this->_network->sendTo(*this->_serverEndpoint, packet);
    this->_sequenceNumber++;
}


void ClientNetwork::disconnectionPacket() {
    if (!_network) {
        throw err::ClientNetworkError("[ClientNetwork] Network not initialized",
            err::ClientNetworkError::INTERNAL_ERROR);
    }
    if (this->_idClient == 0) {
        debug::Debug::printDebug(this->_isDebug,
            "[Client] Warning: Client ID is 0, cannot send disconnection packet",
            debug::debugType::NETWORK,
            debug::debugLevel::WARNING);
        return;
    }
    if (!this->_packet) {
        debug::Debug::printDebug(this->_isDebug,
            "[Client] Warning: Packet manager not initialized, cannot send disconnect packet",
            debug::debugType::NETWORK,
            debug::debugLevel::WARNING);
        return;
    }
    std::vector<uint64_t> payload = {};
    payload.push_back(static_cast<uint64_t>(this->_idClient));

    std::vector<uint8_t> header = this->_packet->pack(this->_idClient,
        this->_sequenceNumber, constants::PACKET_DISC, payload);

    this->_network->sendTo(*this->_serverEndpoint, header);
    this->_sequenceNumber++;
}

void ClientNetwork::sendDisconnectFromLobby() {
    if (!_network) {
        throw err::ClientNetworkError("[ClientNetwork] Network not initialized",
            err::ClientNetworkError::INTERNAL_ERROR);
    }
    if (this->_idClient == 0) {
        debug::Debug::printDebug(this->_isDebug,
            "[Client] Warning: Client ID is 0, cannot send disconnection packet",
            debug::debugType::NETWORK,
            debug::debugLevel::WARNING);
        return;
    }
    if (!this->_packet) {
        debug::Debug::printDebug(this->_isDebug,
            "[Client] Warning: Packet manager not initialized, cannot send disconnect packet",
            debug::debugType::NETWORK,
            debug::debugLevel::WARNING);
        return;
    }
    std::vector<uint64_t> payload = {};
    payload.push_back(static_cast<uint64_t>(this->_idClient));

    std::vector<uint8_t> header = this->_packet->pack(this->_idClient,
        this->_sequenceNumber, constants::PACKET_LEAVE_LOBBY, payload);

    this->_network->sendTo(*this->_serverEndpoint, header);
    this->_sequenceNumber++;
}

void ClientNetwork::sendWhoAmI() {
    if (!_network) {
        throw err::ClientNetworkError("[ClientNetwork] Network not initialized",
            err::ClientNetworkError::INTERNAL_ERROR);
    }
    if (this->_idClient == 0) {
        debug::Debug::printDebug(this->_isDebug,
            "[Client] Warning: Client ID is 0, cannot send WHOAMI packet",
            debug::debugType::NETWORK,
            debug::debugLevel::WARNING);
        return;
    }
    std::vector<uint64_t> payload = {};
    std::vector<uint8_t> packet = this->_packet->pack(this->_idClient,
        this->_sequenceNumber, constants::PACKET_WHOAMI, payload);

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Sending WHOAMI packet",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);

    this->_network->sendTo(*this->_serverEndpoint, packet);
    this->_sequenceNumber++;
}

void ClientNetwork::requestCode() {
    if (!_network) {
        throw err::ClientNetworkError("[ClientNetwork] Network not initialized",
            err::ClientNetworkError::INTERNAL_ERROR);
    }
    if (this->_idClient == 0) {
        debug::Debug::printDebug(this->_isDebug,
            "[Client] Warning: Client ID is 0, cannot send request code packet",
            debug::debugType::NETWORK,
            debug::debugLevel::WARNING);
        return;
    }
    std::vector<uint64_t> payload = {};
    std::vector<uint8_t> packet = this->_packet->pack(this->_idClient,
        this->_sequenceNumber, constants::PACKET_REQUEST_LOBBY, payload);

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Sending request code packet",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    this->_network->sendTo(*this->_serverEndpoint, packet);
    this->_sequenceNumber++;
}

void ClientNetwork::sendLobbyConnection(std::string lobbyCode) {
    if (!this->_network) {
        throw err::ClientNetworkError("[ClientNetwork] Network not initialized",
            err::ClientNetworkError::INTERNAL_ERROR);
    }
    if (this->_idClient == 0) {
        debug::Debug::printDebug(this->_isDebug,
            "[Client] Warning: Client ID is 0, cannot send lobby connection packet",
            debug::debugType::NETWORK,
            debug::debugLevel::WARNING);
        return;
    }

    if (lobbyCode.empty()) {
        lobbyCode = constants::LOBBY_LEAVE_MARKER;
    }

    std::vector<uint64_t> payload;
    for (char c : lobbyCode) {
        payload.push_back(static_cast<uint64_t>(c));
    }

    std::vector<uint8_t> packet = this->_packet->pack(this->_idClient,
        this->_sequenceNumber, constants::PACKET_CONNECT_TO_LOBBY, payload);

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Sending lobby " + std::string(lobbyCode ==
            constants::LOBBY_LEAVE_MARKER ? "leave" : "connection") + " packet",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);

    this->_lobbyCode = lobbyCode;
    this->_network->sendTo(*this->_serverEndpoint, packet);
    this->_sequenceNumber++;
}

void ClientNetwork::sendMasterStartGame() {
    if (!this->_network) {
        throw err::ClientNetworkError("[ClientNetwork] Network not initialized",
            err::ClientNetworkError::INTERNAL_ERROR);
    }
    if (this->_idClient == 0) {
        debug::Debug::printDebug(this->_isDebug,
            "[Client] Warning: Client ID is 0, cannot send start game packet",
            debug::debugType::NETWORK,
            debug::debugLevel::WARNING);
        return;
    }
    std::vector<uint64_t> payload;
    for (char c : this->_lobbyCode) {
        payload.push_back(static_cast<uint64_t>(c));
    }
    std::vector<uint8_t> packet = this->_packet->pack(this->_idClient,
        this->_sequenceNumber, constants::PACKET_LOBBY_MASTER_REQUEST_START, payload);
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Sending start game packet as lobby master",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);

    this->_network->sendTo(*this->_serverEndpoint, packet);
    this->_sequenceNumber++;
}

void ClientNetwork::sendRegisterPacket(const std::string &username,
    const std::string &password) {
    if (!_network) {
        throw err::ClientNetworkError("[ClientNetwork] Network not initialized",
            err::ClientNetworkError::INTERNAL_ERROR);
    }

    std::vector<uint64_t> payload;
    std::vector<uint64_t> usernameBytes = this->_packet->formatString(username);
    std::vector<uint64_t> passwordBytes = this->_packet->formatString(password);
    payload.insert(payload.end(), usernameBytes.begin(), usernameBytes.end());
    payload.insert(payload.end(), passwordBytes.begin(), passwordBytes.end());

    std::vector<uint8_t> packet = this->_packet->pack(this->_idClient,
        this->_sequenceNumber, constants::PACKET_REGISTER, payload);

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Sending register packet for username: " + username,
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    this->_network->sendTo(*this->_serverEndpoint, packet);
    this->_sequenceNumber++;
    this->_expectingRegisterResponse = true;
}

void ClientNetwork::sendLoginPacket(const std::string &username, const std::string &password) {
    if (!_network) {
        throw err::ClientNetworkError("[ClientNetwork] Network not initialized",
            err::ClientNetworkError::INTERNAL_ERROR);
    }

    std::vector<uint64_t> payload;
    std::vector<uint64_t> usernameBytes = this->_packet->formatString(username);
    std::vector<uint64_t> passwordBytes = this->_packet->formatString(password);
    payload.insert(payload.end(), usernameBytes.begin(), usernameBytes.end());
    payload.insert(payload.end(), passwordBytes.begin(), passwordBytes.end());

    std::vector<uint8_t> packet = this->_packet->pack(this->_idClient,
        this->_sequenceNumber, constants::PACKET_LOGIN, payload);

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Sending login packet for username: " + username,
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);

    this->_network->sendTo(*this->_serverEndpoint, packet);
    this->_sequenceNumber++;
    this->_expectingLoginResponse = true;
}

void ClientNetwork::sendRequestLeaderboardPacket() {
    if (!_network) {
        throw err::ClientNetworkError("[ClientNetwork] Network not initialized",
            err::ClientNetworkError::INTERNAL_ERROR);
    }

    std::vector<uint8_t> packet = this->_packet->pack(this->_idClient,
        this->_sequenceNumber, constants::PACKET_REQUEST_LEADERBOARD, {});

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Sending request leaderboard packet",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);

    this->_network->sendTo(*this->_serverEndpoint, packet);
    this->_sequenceNumber++;
    this->_expectingLoginResponse = true;
}

void ClientNetwork::sendRequestProfilePacket() {
    if (!_network) {
        throw err::ClientNetworkError("[ClientNetwork] Network not initialized",
            err::ClientNetworkError::INTERNAL_ERROR);
    }

    std::vector<uint8_t> packet = this->_packet->pack(this->_idClient,
        this->_sequenceNumber, constants::PACKET_REQUEST_PROFILE, {});

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Sending request profile packet",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);

    this->_network->sendTo(*this->_serverEndpoint, packet);
    this->_sequenceNumber++;
    this->_expectingProfileResponse = true;
}

void ClientNetwork::sendMessageToServer(const std::string &message) {
    if (!_network) {
        throw err::ClientNetworkError("[ClientNetwork] Network not initialized",
            err::ClientNetworkError::INTERNAL_ERROR);
    }

    std::vector<std::vector<uint64_t>> formatMessage;
    for (size_t i = 0; i < message.size(); i += 8) {
        std::string _tempMessage;
        for (size_t j = 0; j < 8 && (i + j < message.size()); ++j) {
            char c = message[i + j];
            _tempMessage += c;
        }
        std::vector<uint64_t> formattedPart = this->_packet->formatString(_tempMessage);
        formatMessage.push_back(formattedPart);
    }

    std::vector<uint64_t> payload;
    for (const auto &part : formatMessage) {
        payload.insert(payload.end(), part.begin(), part.end());
    }

    std::vector<uint8_t> packet = this->_packet->pack(this->_idClient,
        this->_sequenceNumber, constants::PACKET_NEW_CHAT, payload);

    if (!this->_network->sendTo(*this->_serverEndpoint, packet)) {
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] Failed to send chat message to server",
            debug::debugType::NETWORK,
            debug::debugLevel::ERROR);
    }
}

void ClientNetwork::sendRequestGameRulesUpdate(uint8_t ruleType, uint8_t value) {
    if (!_network) {
        throw err::ClientNetworkError("[ClientNetwork] Network not initialized",
            err::ClientNetworkError::INTERNAL_ERROR);
    }

    if (this->_idClient == 0) {
        debug::Debug::printDebug(this->_isDebug,
            "[Client] Warning: Client ID is 0, cannot send game rules update request",
            debug::debugType::NETWORK,
            debug::debugLevel::WARNING);
        return;
    }

    std::vector<uint64_t> payload =
        {static_cast<uint64_t>(ruleType), static_cast<uint64_t>(value)};
    std::vector<uint8_t> packet = this->_packet->pack(this->_idClient,
        this->_sequenceNumber, constants::PACKET_REQUEST_GAME_RULES_UPDATE, payload);

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Sending request game rules update packet (type: " +
        std::to_string(ruleType) + ", value: " + std::to_string(value) + ")",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);

    this->_network->sendTo(*this->_serverEndpoint, packet);
    this->_sequenceNumber++;
}
