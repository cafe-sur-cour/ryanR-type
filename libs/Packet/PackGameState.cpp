/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Pack Game State
*/

#include <vector>
#include <memory>
#include "PacketManager.hpp"
#include "../../common/translationToECS.hpp"

std::vector<uint8_t> pm::PacketManager::packPlayerTag(std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i) {
    std::vector<uint8_t> temp = {};
    std::vector<uint8_t> packet = {};
    if (payload.at(*i) == PLAYER_TAG) {
        temp = this->_serializer->serializeUChar(payload.at(*i));
        packet.insert(packet.end(), temp.begin(), temp.end());
        *i += 1;
    }
    return packet;
}

std::vector<uint8_t> pm::PacketManager::packTransformation(std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i) {
    std::vector<uint8_t> temp = {};
    std::vector<uint8_t> packet = {};
    if (payload.at(*i) == TRANSFORM) {
        temp = this->_serializer->serializeUChar(payload.at(*i));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 1));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 2));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 3));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 4));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 5));
        packet.insert(packet.end(), temp.begin(), temp.end());
        *i += 6;
    }
    return packet;
}

std::vector<uint8_t> pm::PacketManager::packVelocity(std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i) {
    std::vector<uint8_t> temp = {};
    std::vector<uint8_t> packet = {};
    if (payload.at(*i) == VELOCITY) {
        temp = this->_serializer->serializeUChar(payload.at(*i));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 1));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 2));
        packet.insert(packet.end(), temp.begin(), temp.end());
        *i += 3;
    }
    return packet;
}

std::vector<uint8_t> pm::PacketManager::packSpeed(std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i) {
    std::vector<uint8_t> temp = {};
    std::vector<uint8_t> packet = {};
    if (payload.at(*i) == SPEED) {
        temp = this->_serializer->serializeUChar(payload.at(*i));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 1));
        packet.insert(packet.end(), temp.begin(), temp.end());
        *i += 2;
    }
    return packet;
}

std::vector<uint8_t> pm::PacketManager::packHealth(std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i) {
    std::vector<uint8_t> temp = {};
    std::vector<uint8_t> packet = {};
    if (payload.at(*i) == HEALTH) {
        temp = this->_serializer->serializeUChar(payload.at(*i));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 1));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 2));
        packet.insert(packet.end(), temp.begin(), temp.end());
        *i += 3;
    }
    return packet;
}

std::vector<uint8_t> pm::PacketManager::packCollider(std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i) {
    std::vector<uint8_t> temp = {};
    std::vector<uint8_t> packet = {};
    if (payload.at(*i) == COLLIDER) {
        temp = this->_serializer->serializeUChar(payload.at(*i));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 1));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 2));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 3));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 4));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeUChar(payload.at(*i + 5));
        packet.insert(packet.end(), temp.begin(), temp.end());
        *i += 6;
    }
    return packet;
}

std::vector<uint8_t> pm::PacketManager::packAIMovementPattern(std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i) {
    std::vector<uint8_t> temp = {};
    std::vector<uint8_t> packet = {};
    if (payload.at(*i) == AI_MOVEMENT_PATTERN) {
        temp = this->_serializer->serializeUChar(payload.at(*i));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 1));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 2));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 3));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 4));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 5));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 6));
        packet.insert(packet.end(), temp.begin(), temp.end());
        *i += 7;
    }
    return packet;
}