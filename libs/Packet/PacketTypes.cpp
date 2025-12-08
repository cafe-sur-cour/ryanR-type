/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Packet Types
*/

#include <iostream>
#include <vector>
#include "PacketManager.hpp"

std::vector<std::uint8_t> pm::PacketManager::buildConnectionPacket(
    std::vector<std::uint64_t> payload) {
    std::vector<std::uint8_t> body;
    std::vector<uint8_t> temp;

    for (uint8_t i = 0; i < LENGTH_CONNECTION_PACKET; i++) {
        temp = this->_serializer->serializeUChar(payload.at(i));
        body.insert(body.end(), temp.begin(), temp.end());
    }
    return body;
}

bool pm::PacketManager::parseConnectionPacket(
    const std::vector<std::uint8_t> payload) {
    if (payload.size() != LENGTH_CONNECTION_PACKET ||
        LENGTH_CONNECTION_PACKET!= payload.size()) {
        return false;
    }

    for (size_t i = 0; i < LENGTH_CONNECTION_PACKET; i++)
        this->_payload.push_back(payload.at(i));
    return true;
}

std::vector<uint8_t> pm::PacketManager::buildAcceptationPacket(
    std::vector<uint64_t> payload) {
    std::vector<uint8_t> body;
    std::vector<uint8_t> temp;

    temp = this->_serializer->serializeUChar(payload.at(0));
    body.insert(body.end(), temp.begin(), temp.end());
    return body;
}

bool pm::PacketManager::parseAcceptationPacket(const std::vector<uint8_t> payload) {
    if (payload.size() != LENGTH_ACCEPTATION_PACKET) {
        return false;
    }

    std::vector<uint8_t> charBytes(payload.begin() + 0, payload.begin() + 1);
    uint64_t result = this->_serializer->deserializeUChar(charBytes);
    this->_payload.push_back(result);
    return true;
}

std::vector<uint8_t> pm::PacketManager::buildDisconnectionPacket(
    std::vector<uint64_t> payload) {
    std::vector<uint8_t> body;
    std::vector<uint8_t> temp;

    temp = this->_serializer->serializeUChar(payload.at(0));
    body.insert(body.end(), temp.begin(), temp.end());
    return body;
}

bool pm::PacketManager::parseDisconnectionPacket(
    const std::vector<std::uint8_t> payload) {
    if (payload.size() != LENGTH_DISCONNECTION_PACKET) {
        return false;
    }

    std::vector<std::uint8_t> charBytes(
        payload.begin() + 0, payload.begin() + 1);
    uint64_t result = this->_serializer->deserializeUChar(charBytes);
    this->_payload.push_back(result);
    return true;
}

std::vector<uint8_t> pm::PacketManager::buildEventPacket(
    std::vector<uint64_t> payload) {
    std::vector<uint8_t> body;
    std::vector<uint8_t> temp;

    temp = this->_serializer->serializeUChar(payload.at(0));
    body.insert(body.end(), temp.begin(), temp.end());
    temp = this->_serializer->serializeULong(payload.at(1));
    body.insert(body.end(), temp.begin(), temp.end());
    temp = this->_serializer->serializeULong(payload.at(2));
    body.insert(body.end(), temp.begin(), temp.end());
    return body;
}

bool pm::PacketManager::parseEventPacket(const std::vector<uint8_t> payload) {
    if (payload.size() != LENGTH_EVENT_PACKET) {
        return false;
    }

    std::vector<uint8_t> charBytes1(
        payload.begin() + 0, payload.begin() + 1);
    uint64_t result1 = this->_serializer->deserializeUChar(charBytes1);
    this->_payload.push_back(result1);

    std::vector<uint8_t> longBytes1(
        payload.begin() + 1, payload.begin() + 10);
    uint64_t result3 = this->_serializer->deserializeULong(longBytes1);
    this->_payload.push_back(result3);

    std::vector<uint8_t> longBytes2(
        payload.begin() + 10, payload.begin() + 18);
    uint64_t result4 = this->_serializer->deserializeULong(longBytes2);
    this->_payload.push_back(result4);
    return true;
}

std::vector<uint8_t> pm::PacketManager::buildMapPacket(std::vector<uint64_t> payload) {
    std::vector<uint8_t> body;
    for (auto val : payload) {
        auto temp = this->_serializer->serializeULong(val);
        body.insert(body.end(), temp.begin(), temp.end());
    }
    return body;
}


bool pm::PacketManager::parseMapPacket(const std::vector<uint8_t> payload) {
    if (payload.size() % 8 != 0) {
        std::cerr << "[PACKET] MAP packet payload size not multiple of 8: "
                  << payload.size() << std::endl;
        return false;
    }

    this->_payload.clear();
    for (size_t i = 0; i < payload.size(); i += 8) {
        auto startIt = payload.begin() + static_cast<std::ptrdiff_t>(i);
        auto endIt = payload.begin() + static_cast<std::ptrdiff_t>(i + 8);
        std::vector<uint8_t> longBytes(startIt, endIt);

        uint64_t value = this->_serializer->deserializeULong(longBytes);
        this->_payload.push_back(value);
    }

    return true;
}
