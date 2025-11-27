/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Packet Types
*/

#include <iostream>
#include <vector>
#include "PacketManager.hpp"

std::vector<std::uint8_t> PacketManager::buildConnectionPacket(
    std::vector<std::uint64_t> payload) {
    std::vector<std::uint8_t> body;
    std::vector<uint8_t> temp;

    for (uint8_t i = 0; i < (LENGTH_CONNECTION_PACKET - LENGTH_EOP); i++) {
        temp = this->_serializer->serializeUChar(payload.at(i));
        body.insert(body.end(), temp.begin(), temp.end());
    }
    temp = this->_serializer->serializeUChar(this->_firstEndOfPacket);
    body.insert(body.end(), temp.begin(), temp.end());
    temp = this->_serializer->serializeUChar(this->_secondEndOfPacket);
    body.insert(body.end(), temp.begin(), temp.end());
    return body;
}

bool PacketManager::parseConnectionPacket(
    const std::vector<std::uint8_t> payload) {
    if (payload.size() != LENGTH_CONNECTION_PACKET ||
        this->_length != payload.size()) {
        std::cerr <<
            "[PACKET] Error: Payload have invalid length"
            << std::endl;
        return false;
    }

    for (size_t i = 1; i < (LENGTH_CONNECTION_PACKET - LENGTH_EOP); i++)
        this->_payload.push_back(payload.at(i));
    return true;
}

std::vector<uint8_t> PacketManager::buildAcceptationPacket(
    std::vector<uint64_t> payload) {
    std::vector<uint8_t> body;
    std::vector<uint8_t> temp;

    temp = this->_serializer->serializeUChar(payload.at(0));
    body.insert(body.end(), temp.begin(), temp.end());
    temp = this->_serializer->serializeUChar(payload.at(1));
    body.insert(body.end(), temp.begin(), temp.end());
    temp = this->_serializer->serializeUChar(this->_firstEndOfPacket);
    body.insert(body.end(), temp.begin(), temp.end());
    temp = this->_serializer->serializeUChar(this->_secondEndOfPacket);
    body.insert(body.end(), temp.begin(), temp.end());
    return body;
}

bool PacketManager::parseAcceptationPacket(const std::vector<uint8_t> payload) {
    if (payload.size() != LENGTH_ACCEPTATION_PACKET
        || this->_length != payload.size()) {
        std::cerr <<
            "[PACKET] Error: Payload have invalid length"
            << std::endl;
        return false;
    }

    std::vector<uint8_t> charBytes(payload.begin() + 1, payload.begin() + 2);
    uint64_t result = this->_serializer->deserializeUChar(charBytes);
    this->_payload.push_back(result);
    return true;
}

std::vector<uint8_t> PacketManager::buildDisconnectionPacket(
    std::vector<uint64_t> payload) {
    std::vector<uint8_t> body;
    std::vector<uint8_t> temp;

    temp = this->_serializer->serializeUChar(payload.at(0));
    body.insert(body.end(), temp.begin(), temp.end());
    temp = this->_serializer->serializeUChar(payload.at(1));
    body.insert(body.end(), temp.begin(), temp.end());
    temp = this->_serializer->serializeUChar(this->_firstEndOfPacket);
    body.insert(body.end(), temp.begin(), temp.end());
    temp = this->_serializer->serializeUChar(this->_secondEndOfPacket);
    body.insert(body.end(), temp.begin(), temp.end());
    return body;
}

bool PacketManager::parseDisconnectionPacket(
    const std::vector<std::uint8_t> payload) {
    if (payload.size() != LENGTH_DISCONNECTION_PACKET
        || this->_length != payload.size()) {
        std::cerr <<
            "[PACKET] Error: Payload have invalid length"
            << std::endl;
        return false;
    }

    std::vector<std::uint8_t> charBytes(
        payload.begin() + 1, payload.begin() + 2);
    uint64_t result = this->_serializer->deserializeUChar(charBytes);
    this->_payload.push_back(result);
    return true;
}

std::vector<uint8_t> PacketManager::buildEventPacket(
    std::vector<uint64_t> payload) {
    std::vector<uint8_t> body;
    std::vector<uint8_t> temp;

    temp = this->_serializer->serializeUChar(payload.at(0));
    body.insert(body.end(), temp.begin(), temp.end());
    temp = this->_serializer->serializeUChar(payload.at(1));
    body.insert(body.end(), temp.begin(), temp.end());
    temp = this->_serializer->serializeUChar(payload.at(2));
    body.insert(body.end(), temp.begin(), temp.end());
    temp = this->_serializer->serializeUChar(this->_firstEndOfPacket);
    body.insert(body.end(), temp.begin(), temp.end());
    temp = this->_serializer->serializeUChar(this->_secondEndOfPacket);
    body.insert(body.end(), temp.begin(), temp.end());
    return body;
}

bool PacketManager::parseEventPacket(const std::vector<uint8_t> payload) {
    if (payload.size() != LENGTH_EVENT_PACKET
        || this->_length != payload.size()) {
        std::cerr <<
            "[PACKET] Error: Payload have invalid length"
            << std::endl;
        return false;
    }

    std::vector<uint8_t> charBytes1(
        payload.begin() + 1, payload.begin() + 2);
    uint64_t result1 = this->_serializer->deserializeUChar(charBytes1);
    this->_payload.push_back(result1);

    std::vector<uint8_t> charBytes2(
        payload.begin() + 2, payload.begin() + 3);
    uint64_t result2 = this->_serializer->deserializeUChar(charBytes2);
    this->_payload.push_back(result2);
    return true;
}
