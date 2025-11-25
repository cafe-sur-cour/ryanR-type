/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Packet
*/

#include <vector>
#include <map>
#include <iostream>
#include <memory>
#include <string>
#include "Packet.hpp"
#include "../../common/DLLoader/LoaderType.hpp"
#include "serializer/BigEndianSerialization.hpp"

Packet::Packet(int seqNumber) {
    this->_magicNumber = MAGIC_NUMBER;
    this->_sequenceNumber = seqNumber;
    this->_type = NO_OP_PACKET;
    this->_length = 0;
    this->_endOfPacket = (FIRST_EOP_CHAR << 8) | SECOND_EOP_CHAR;
    this->_serializer = std::make_shared<BigEndianSerialization>();

    this->_packetHandlers = {
        {0x01, std::bind(&Packet::connectionPacket,
            this, std::placeholders::_1)},
        {0x02, std::bind(&Packet::disconnectionPacket,
            this, std::placeholders::_1)},
        {0x03, std::bind(&Packet::eventPacket,
            this, std::placeholders::_1)}
    };

    this->_packetLengths = {
        {CONNECTION_CLIENT_PACKET, LENGTH_CONNECTION_PACKET},
        {DISCONNECTION_PACKET, LENGTH_DISCONNECTION_PACKET},
        {EVENT_PACKET, LENGTH_EVENT_PACKET}
    };
}

Packet::~Packet() {
    if (this->_serializer) {
        this->_serializer.reset();
    }
}

uint8_t Packet::getMagicNumber() const {
    return this->_magicNumber;
}

size_t Packet::getLength() const {
    return this->_length;
}

size_t Packet::getSequenceNumber() const {
    return this->_sequenceNumber;
}

uint8_t Packet::getType() const {
    return this->_type;
}

void Packet::setType(uint8_t type) {
    this->_type = type;
}

void Packet::setLength(size_t length) {
    this->_length = length;
}

std::vector<uint8_t> Packet::packHeaderPacket(unsigned int idClient,
    unsigned int sequenceNumber, uint8_t type) {
    std::vector<uint8_t> header;
    std::vector<uint8_t> temp;
    unsigned int length = 0;

    temp = this->_serializer->serializeChar(this->_magicNumber);
    header.insert(header.end(), temp.begin(), temp.end());

    temp = this->_serializer->serializeInt(idClient);
    header.insert(header.end(), temp.begin(), temp.end());

    temp = this->_serializer->serializeInt(sequenceNumber);
    header.insert(header.end(), temp.begin(), temp.end());

    temp = this->_serializer->serializeChar(type);
    header.insert(header.end(), temp.begin(), temp.end());

    for (auto &pair : this->_packetLengths) {
        if (pair.first == type) {
            length = pair.second;
            break;
        }
    }
    temp = this->_serializer->serializeInt(length);
    header.insert(header.end(), temp.begin(), temp.end());

    temp = this->_serializer->serializeShort(this->_endOfPacket);
    header.insert(header.end(), temp.begin(), temp.end());
    return header;
}

std::vector<uint8_t> Packet::packBodyPacket(std::vector<std::uint8_t> payload) {
    std::vector<uint8_t> body;
    uint8_t type = NO_OP_PACKET;

    if (payload.empty()) {
        return body;
    }

    type = payload.at(0);
    for (auto &handler : this->_packetHandlers) {
        if (handler.first == type) {
            body = handler.second(payload);
            break;
        }
    }
    return body;
}

bool Packet::unpackPacket(std::vector<uint8_t> data) {
    (void)data;
    return false;
}

extern "C" {
    int getType() {
        return PACKET_MODULE;
    }
}
