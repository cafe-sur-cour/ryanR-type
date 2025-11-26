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

Packet::Packet(unsigned int sequenceNumber) {
    this->_magicNumber = MAGIC_NUMBER;
    this->_idClient = 0;
    this->_sequenceNumber = sequenceNumber;
    this->_type = NO_OP_PACKET;
    this->_length = 0;
    this->_endOfPacket = (FIRST_EOP_CHAR << 8) | SECOND_EOP_CHAR;
    this->_payload = std::vector<std::uint8_t>();
    this->_serializer = std::make_shared<BigEndianSerialization>();

    this->_packetHandlers = {
        {CONNECTION_CLIENT_PACKET, std::bind(&Packet::connectionPacket,
            this, std::placeholders::_1)},
        {DISCONNECTION_PACKET, std::bind(&Packet::disconnectionPacket,
            this, std::placeholders::_1)},
        {EVENT_PACKET, std::bind(&Packet::eventPacket,
            this, std::placeholders::_1)}
    };

    this->_packetReceived = {
        {CONNECTION_SERVER_PACKET, std::bind(&Packet::parseConnectionPacket,
            this, std::placeholders::_1)}
    };

    this->_packetLengths = {
        {CONNECTION_CLIENT_PACKET, LENGTH_CONNECTION_PACKET},
        {CONNECTION_SERVER_PACKET, LENGTH_CONNECTION_SERVER_PACKET},
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

std::vector<std::uint8_t> Packet::getPayload() const {
    return this->_payload;
}

void Packet::setType(uint8_t type) {
    this->_type = type;
}

void Packet::setLength(size_t length) {
    this->_length = length;
}

std::vector<uint8_t> Packet::packHeaderPacket(int idClient,
    unsigned int sequenceNumber, uint8_t type) {
    std::vector<uint8_t> header;
    std::vector<uint8_t> temp;
    unsigned int length = 0;

    temp = this->_serializer->serializeChar(this->_magicNumber);
    header.insert(header.end(), temp.begin(), temp.end());

    temp = this->_serializer->serializeInt(idClient);
    header.insert(header.end(), temp.begin(), temp.end());

    (void)sequenceNumber;
    //  temp = this->_serializer->serializeInt(sequenceNumber);
    //  header.insert(header.end(), temp.begin(), temp.end());

    temp = this->_serializer->serializeChar(type);
    header.insert(header.end(), temp.begin(), temp.end());

    for (auto &pair : this->_packetLengths) {
        if (pair.first == type) {
            length = pair.second;
            break;
        }
    }
    (void)length;
    // temp = this->_serializer->serializeInt(length);
    // header.insert(header.end(), temp.begin(), temp.end());

    // temp = this->_serializer->serializeShort(this->_endOfPacket);
    // header.insert(header.end(), temp.begin(), temp.end());
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
    if (data.empty()) {
        return false;
    }

    if (data.at(0) == this->_magicNumber) {
        this->_idClient = (data.at(1) << 24) | (data.at(2) << 16) |
            (data.at(3) << 8) | data.at(4);
        this->_sequenceNumber = (data.at(5) << 24) | (data.at(6) << 16) |
            (data.at(7) << 8) | data.at(8);
        this->_type = data.at(9);
        this->_length = (data.at(10) << 24) | (data.at(11) << 16) |
            (data.at(12) << 8) | data.at(13);
        return true;
    } else {
        if (this->_type == data.at(0)) {
            for (auto &received : this->_packetReceived) {
                if (received.first == this->_type) {
                    received.second(data);
                    return true;
                }
            }
            return false;
        }
        return false;
    }
}

extern "C" {

    void *createPacketInstance(unsigned int id) {
        return new Packet(id);
    }
    int getType() {
        return PACKET_MODULE;
    }
}
