/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Packet Types
*/

#include <iostream>
#include <vector>
#include "Packet.hpp"

std::vector<std::uint8_t> Packet::sendConnectionPacket(
    std::vector<std::uint8_t> payload) {
    std::vector<std::uint8_t> body;
    std::vector<uint8_t> temp;

    for (int i = 0; i < (LENGTH_CONNECTION_PACKET - LENGTH_EO_PACKET); i++) {
        temp = this->_serializer->serializeChar(payload.at(i));
        body.insert(body.end(), temp.begin(), temp.end());
    }
    temp = this->_serializer->serializeShort(this->_endOfPacket);
    body.insert(body.end(), temp.begin(), temp.end());
    return body;
}

bool Packet::parseConnectionPacket(const std::vector<std::uint8_t> payload) {
    if (payload.size() != LENGTH_CONNECTION_PACKET) {
        std::cerr << "[SERVER] Error: parseConnectionPacket(): Payload have invalid length" << std::endl;
        return false;
    }

    for (size_t i = 1; i < 9; i++)
        this->_payload.push_back(payload.at(i));
    return true;
}

std::vector<std::uint8_t> Packet::sendAcceptationPacket(
    std::vector<std::uint8_t> payload) {
    std::vector<std::uint8_t> body;
    std::vector<uint8_t> temp;

    temp = this->_serializer->serializeChar(payload.at(0));
    body.insert(body.end(), temp.begin(), temp.end());
    temp = this->_serializer->serializeInt(payload.at(1));
    body.insert(body.end(), temp.begin(), temp.end());
    temp = this->_serializer->serializeShort(this->_endOfPacket);
    body.insert(body.end(), temp.begin(), temp.end());
    return body;
}

bool Packet::parseAcceptationPacket(const std::vector<std::uint8_t> payload) {
    if (payload.size() != LENGTH_ACCEPTATION_PACKET) {
        std::cerr << "[SERVER] Error: parseAcceptationPacket(): Payload have invalid length" << std::endl;
        return false;
    }

    std::vector<std::uint8_t> intBytes(
        payload.begin() + 1, payload.begin() + 5);
    std::uint8_t result = this->_serializer->deserializeInt(intBytes);
    this->_payload.push_back(result);
    return true;
}

std::vector<std::uint8_t> Packet::sendDisconnectionPacket(
    std::vector<std::uint8_t> payload) {
    std::vector<std::uint8_t> body;
    std::vector<uint8_t> temp;

    temp = this->_serializer->serializeChar(payload.at(0));
    body.insert(body.end(), temp.begin(), temp.end());
    temp = this->_serializer->serializeInt(payload.at(1));
    body.insert(body.end(), temp.begin(), temp.end());
    temp = this->_serializer->serializeShort(this->_endOfPacket);
    body.insert(body.end(), temp.begin(), temp.end());
    return body;
}

bool Packet::parseDisconnectionPacket(const std::vector<std::uint8_t> payload) {
    if (payload.size() != LENGTH_DISCONNECTION_PACKET) {
        std::cerr << "[SERVER] Error: parseDisconnectionPacket(): Payload have invalid length" << std::endl;
        return false;
    }

    std::vector<std::uint8_t> intBytes(
        payload.begin() + 1, payload.begin() + 5);
    std::uint8_t result = this->_serializer->deserializeInt(intBytes);
    this->_payload.push_back(result);
    return true;
}

std::vector<std::uint8_t> Packet::sendEventPacket(
    std::vector<std::uint8_t> payload) {
    std::vector<std::uint8_t> body;
    std::vector<uint8_t> temp;

    temp = this->_serializer->serializeChar(payload.at(0));
    body.insert(body.end(), temp.begin(), temp.end());
    temp = this->_serializer->serializeInt(payload.at(1));
    body.insert(body.end(), temp.begin(), temp.end());
    temp = this->_serializer->serializeChar(payload.at(2));
    body.insert(body.end(), temp.begin(), temp.end());
    temp = this->_serializer->serializeShort(this->_endOfPacket);
    body.insert(body.end(), temp.begin(), temp.end());
    return body;
}

bool Packet::parseEventPacket(const std::vector<std::uint8_t> payload) {
    if (payload.size() != LENGTH_EVENT_PACKET) {
        std::cerr << "[SERVER] Error: parseEventPacket(): Payload have invalid length" << std::endl;
        return false;
    }

    std::vector<std::uint8_t> intBytes1(
        payload.begin() + 1, payload.begin() + 5);
    std::uint8_t result1 = this->_serializer->deserializeInt(intBytes1);
    this->_payload.push_back(result1);

    std::vector<std::uint8_t> charBytes(
        payload.begin() + 5, payload.begin() + 6);
    std::uint8_t result2 = this->_serializer->deserializeChar(charBytes);
    this->_payload.push_back(result2);
}