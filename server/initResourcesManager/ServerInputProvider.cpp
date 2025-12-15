/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ServerInputProvider
*/

#include "ServerInputProvider.hpp"
#include <utility>
#include <map>
#include <iostream>
#include <algorithm>
#include <vector>
#include "../../common/constants.hpp"

namespace ecs {

ServerInputProvider::ServerInputProvider() {
    _inputHandlers = {
        &ServerInputProvider::handleUp,
        &ServerInputProvider::handleDown,
        &ServerInputProvider::handleLeft,
        &ServerInputProvider::handleRight,
        &ServerInputProvider::handleShoot,
        &ServerInputProvider::handleStop
    };
    this->_inputMapping = std::vector<std::tuple<size_t, size_t, InputMapping>>();
    this->_clientAxisValues = std::map<size_t, std::map<ecs::InputAction, float>>();
}

float ServerInputProvider::getAxisValue(event_t axis, size_t clientID) {
    (void)axis;
    (void)clientID;
    return 0.0f;
}

bool ServerInputProvider::isActionPressed(InputAction action, size_t clientID) {
    (void)action;
    (void)clientID;
    return false;
}

float ServerInputProvider::getActionAxis(InputAction action, size_t clientID) {
    auto clientIt = _clientAxisValues.find(clientID);

    if (clientIt == _clientAxisValues.end()) {
        return 0.0f;
    }
    auto actionIt = clientIt->second.find(action);
    if (actionIt != clientIt->second.end()) {
        float value = actionIt->second;
        return value;
    }
    return 0.0f;
}

InputMapping ServerInputProvider::getInputMapping(size_t clientID) const {
    for (const auto& [id, identity, mapping] : _inputMapping) {
        if (id == clientID) {
            return mapping;
        }
    }
    return InputMapping();
}

void ServerInputProvider::setAxisValue(ecs::InputAction action, float value, size_t clientID) {
    _clientAxisValues[clientID][action] = value;
}

void ServerInputProvider::handleUp(size_t clientID, float value) {
    setAxisValue(ecs::InputAction::MOVE_Y, -value, clientID);
}

void ServerInputProvider::handleDown(size_t clientID, float value) {
    setAxisValue(ecs::InputAction::MOVE_Y, value, clientID);
}

void ServerInputProvider::handleLeft(size_t clientID, float value) {
    setAxisValue(ecs::InputAction::MOVE_X, -value, clientID);
}

void ServerInputProvider::handleRight(size_t clientID, float value) {
    setAxisValue(ecs::InputAction::MOVE_X, value, clientID);
}

void ServerInputProvider::handleStop(size_t clientID, float value) {
    (void)value;
    setAxisValue(ecs::InputAction::MOVE_X, 0.0f, clientID);
    setAxisValue(ecs::InputAction::MOVE_Y, 0.0f, clientID);
}

void ServerInputProvider::handleShoot(size_t clientID, float value) {
    setAxisValue(ecs::InputAction::SHOOT, value, clientID);
}



void ServerInputProvider::updateInputFromEvent
(size_t clientID, constants::EventType eventType, float value) {
    size_t index = static_cast<size_t>(eventType);
    if (index < _inputHandlers.size()) {
        (this->*_inputHandlers[index])(clientID, value);
    }
}

std::vector<size_t> ServerInputProvider::getConnectedClients() const {
    std::vector<size_t> clients;
    for (const auto& [clientID, axisMap] : _clientAxisValues) {
        clients.push_back(clientID);
    }
    return clients;
}

void ServerInputProvider::addClientInputMapping
(size_t clientID, size_t identity, const InputMapping& mapping) {
    _inputMapping.emplace_back(clientID, identity, mapping);
}
}  // namespace ecs
