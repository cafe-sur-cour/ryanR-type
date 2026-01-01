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
#include "Server.hpp"
#include "Constants.hpp"
#include "Utils.hpp"
#include "../common/debug.hpp"
#include "../common/translationToECS.hpp"
#include "../common/ECS/entity/Entity.hpp"
#include "../common/ECS/entity/registry/Registry.hpp"
#include "../common/Parser/Parser.hpp"

bool rserv::Server::connectionPacket(asio::ip::udp::endpoint endpoint) {
    std::vector<uint8_t> packet = this->_packet->pack(constants::ID_SERVER,
        this->_sequenceNumber, constants::PACKET_ACCEPT, std::vector<uint64_t>{
        static_cast<uint64_t>(this->_nextClientId)});

    if (!this->_network->sendTo(endpoint, packet)) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER NETWORK] Failed to send connection acceptance header to "
            + endpoint.address().to_string() + ":" + std::to_string(endpoint.port()),
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return false;
    }
    this->_sequenceNumber++;
    return true;
}

bool rserv::Server::gameStatePacket() {
    if (!this->_resourceManager->has<ecs::Registry>()) {
        return false;
    }

    auto registry = this->_resourceManager->get<ecs::Registry>();
    for (auto& client : this->_clients) {
        uint8_t clientId = std::get<0>(client);
        for (ecs::Entity i = 0; i < registry->getMaxEntityId(); i++) {
            uint32_t entityId = static_cast<uint32_t>(i);
            std::vector<uint64_t> componentData;
            for (const auto& func : this->_convertFunctions) {
                std::vector<uint64_t> compData = func(registry, i);
                componentData.insert(componentData.end(), compData.begin(), compData.end());
            }

            EntitySnapshot snapshot = ComponentSerializer::createSnapshotFromComponents(
                entityId, componentData
            );

            std::vector<uint64_t> delta = this->_deltaTracker.createEntityDelta(
                clientId, entityId, snapshot);
            if (delta.empty()) {
                continue;
            }

            std::vector<uint64_t> payload;
            payload.push_back(entityId);
            payload.insert(payload.end(), componentData.begin(), componentData.end());

            std::vector<uint8_t> packet = this->_packet->pack(
                constants::ID_SERVER,
                this->_sequenceNumber,
                constants::PACKET_GAME_STATE,
                payload
            );

            if (!this->_network->sendTo(std::get<1>(client), packet)) {
                debug::Debug::printDebug(this->_config->getIsDebug(),
                    "[SERVER NETWORK] Failed to send game state packet to client " +
                    std::to_string(static_cast<int>(clientId)),
                    debug::debugType::NETWORK, debug::debugLevel::ERROR);
                return false;
            }
        }
    }
    this->_sequenceNumber++;
    return true;
}

bool rserv::Server::canStartPacket() {
    debug::Debug::printDebug(this->_config->getIsDebug(),
        "[SERVER] Checking canStart: clients=" +
        std::to_string(this->_clients.size()) + ", allReady=" +
        std::to_string(this->allClientsReady()),
        debug::debugType::NETWORK, debug::debugLevel::INFO);
    if (this->_clients.size() > 0 && this->allClientsReady()) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] All connected clients are ready, starting game",
            debug::debugType::NETWORK, debug::debugLevel::INFO);

        this->_gameStarted = true;
        std::string playerString = "player";
        auto prefabMgr = _resourceManager->get<EntityPrefabManager>();
        auto registry = _resourceManager->get<ecs::Registry>();
        for (const auto &client : this->_clients) {
            uint8_t clientId = std::get<0>(client);
            ecs::Entity playerEntity = prefabMgr->createEntityFromPrefab(
                playerString,
                registry,
                ecs::EntityCreationContext::forServer()
            );
            debug::Debug::printDebug(this->_config->getIsDebug(),
                "[SERVER] Created player entity " + std::to_string(playerEntity) +
                " for client " + std::to_string(static_cast<int>(clientId)),
                debug::debugType::NETWORK, debug::debugLevel::INFO);
        }

        std::vector<uint64_t> payload;
        for (auto &client : this->_clients) {
            std::vector<uint64_t> name = this->_packet->formatString(std::get<2>(client));
            payload.insert(payload.end(), name.begin(), name.end());
        }
        std::vector<uint8_t> packetStart = this->_packet->pack(constants::ID_SERVER,
            this->_sequenceNumber, constants::PACKET_CAN_START, payload);

        if (!this->_network->broadcast(this->getConnectedClientEndpoints(), packetStart)) {
            debug::Debug::printDebug(this->_config->getIsDebug(),
                "[SERVER NETWORK] Failed to broadcast can start packet",
                debug::debugType::NETWORK, debug::debugLevel::ERROR);
            return false;
        }
        this->_sequenceNumber++;
        return true;
    }
    return false;
}

bool rserv::Server::endGamePacket(bool isWin) {
    std::vector<uint64_t> payload;
    payload.push_back(isWin ? 1 : 0);
    std::vector<uint8_t> packet = this->_packet->pack(constants::ID_SERVER,
        this->_sequenceNumber, constants::PACKET_END_GAME, payload);

    if (!this->_network->broadcast(this->getConnectedClientEndpoints(), packet)) {
        std::cout << "[SERVER NETWORK] Failed to broadcast end game packet" << std::endl;
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER NETWORK] Failed to broadcast end game packet",
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return false;
    }
    this->_sequenceNumber++;
    return true;
}

std::vector<uint64_t> rserv::Server::spawnPacket(
    size_t entityId,
    const std::string prefabName) {
    std::vector<uint64_t> payload;

    payload.push_back(static_cast<uint64_t>(entityId));
    for (const auto &c : prefabName) {
        payload.push_back(static_cast<uint64_t>(c));
    }
    payload.push_back(static_cast<uint64_t>(constants::END_OFSTRING_ST));
    payload.push_back(static_cast<uint64_t>(constants::END_OFSTRING_ND));
    payload.push_back(static_cast<uint64_t>(constants::END_OFSTRING_TRD));
    return payload;
}

std::vector<uint64_t> rserv::Server::deathPacket(size_t entityId) {
    std::vector<uint64_t> payload;

    payload.push_back(static_cast<uint64_t>(entityId));
    return payload;
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

        if (!this->_network->sendTo(std::get<1>(client), packet)) {
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

bool rserv::Server::sendCodeLobbyPacket(asio::ip::udp::endpoint endpoint) {
    /* Create random code */
    std::string lobbyCode;
    bool isUnique = false;
    int maxAttempts = 20;
    int attempts = 0;

    while (!isUnique && attempts < maxAttempts) {
        lobbyCode = Utils::createAlphaNumericCode();
        isUnique = true;
        for (const auto& lobby : this->lobbys) {
            if (lobby.first == lobbyCode) {
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
            "[SERVER NETWORK] Failed to send lobby code packet to "
            + endpoint.address().to_string() + ":" + std::to_string(endpoint.port()),
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return false;
    }
    /* Add to lobby vector, code and client endpoint */
    this->lobbys.push_back(std::make_pair(lobbyCode, endpoint));
    this->_sequenceNumber++;
    return true;
}
