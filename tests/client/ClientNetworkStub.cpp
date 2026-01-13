/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Minimal ClientNetwork stub for testing
*/

#include "../../client/ClientNetwork.hpp"

// Minimal constructor implementation
ClientNetwork::ClientNetwork() {
    _port = 0;
    _ip = "";
    _name = "";
    _idClient = 0;
    _isConnected = false;
    _network = nullptr;
    _receptionBuffer = nullptr;
    _sendBuffer = nullptr;
    _packet = nullptr;
    _sequenceNumber = 0;
    _ready = false;
    _isDebug = false;
    _resourceManager = nullptr;
    _gsm = nullptr;
    _lobbyCode = "";
    _isConnectedToLobby = false;
    _isLobbyMaster = false;
}

// Minimal destructor implementation
ClientNetwork::~ClientNetwork() {
    // Empty - no cleanup needed for testing
}

// Minimal setter implementations
void ClientNetwork::setResourceManager(std::shared_ptr<ResourceManager> resourceManager) {
    _resourceManager = resourceManager;
}

void ClientNetwork::setDebugMode(bool isDebug) {
    _isDebug = isDebug;
}
