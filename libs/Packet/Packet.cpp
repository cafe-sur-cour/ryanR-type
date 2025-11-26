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
#include "serializer/BigEndianSerialization.hpp"
#include "../../common/DLLoader/LoaderType.hpp"
#include "../../common/Error/PacketError.hpp"

Packet::Packet(uint32_t seqNumber) {
    this->_magicNumber = MAGIC_NUMBER;
    this->_idClient = 0;
    this->_sequenceNumber = seqNumber;
    this->_type = NO_OP_PACKET;
    this->_length = 0;
    this->_endOfPacket = (FIRST_EOP_CHAR << 8) | SECOND_EOP_CHAR;
    this->_payload = std::vector<uint64_t>();
    this->_serializer = std::make_shared<BigEndianSerialization>();
    if (!this->_serializer) {
        throw err::PacketError("[Server] Failed to load serializer",
            err::PacketError::LIBRARY_LOAD_FAILED);
    }

    this->_packetHandlers = {
        {CONNECTION_CLIENT_PACKET, std::bind(&Packet::buildConnectionPacket,
            this, std::placeholders::_1)},
        {ACCEPTATION_PACKET, std::bind(&Packet::buildAcceptationPacket,
            this, std::placeholders::_1)},
        {DISCONNECTION_PACKET, std::bind(&Packet::buildDisconnectionPacket,
            this, std::placeholders::_1)},
        {EVENT_PACKET, std::bind(&Packet::buildEventPacket,
            this, std::placeholders::_1)}
    };

    this->_packetReceived = {
        {CONNECTION_CLIENT_PACKET, std::bind(&Packet::parseConnectionPacket,
            this, std::placeholders::_1)},
        {ACCEPTATION_PACKET, std::bind(&Packet::parseAcceptationPacket,
            this, std::placeholders::_1)},
        {DISCONNECTION_PACKET, std::bind(&Packet::parseDisconnectionPacket,
            this, std::placeholders::_1)},
        {EVENT_PACKET, std::bind(&Packet::parseEventPacket,
            this, std::placeholders::_1)}
    };

    this->_packetLengths = {
        {CONNECTION_CLIENT_PACKET, LENGTH_CONNECTION_PACKET},
        {ACCEPTATION_PACKET, LENGTH_ACCEPTATION_PACKET},
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

uint32_t Packet::getLength() const {
    return this->_length;
}

uint32_t Packet::getSequenceNumber() const {
    return this->_sequenceNumber;
}

uint8_t Packet::getType() const {
    return this->_type;
}

std::vector<uint64_t> Packet::getPayload() const {
    return this->_payload;
}

uint8_t Packet::getIdClient() const {
    return this->_idClient;
}



void Packet::setType(uint8_t type) {
    this->_type = type;
}

void Packet::setLength(uint32_t length) {
    this->_length = length;
}

void Packet::setSequenceNumber(uint32_t sequenceNumber) {
    this->_sequenceNumber = sequenceNumber;
}

void Packet::setPayload(std::vector<uint64_t> payload) {
    this->_payload = payload;
}

void Packet::setIdClient(uint8_t idClient) {
    this->_idClient = idClient;
}



void Packet::reset() {
    this->_magicNumber = MAGIC_NUMBER;
    this->_idClient = 0;
    this->_sequenceNumber = 0;
    this->_type = NO_OP_PACKET;
    this->_length = 0;
    this->_endOfPacket = (FIRST_EOP_CHAR << 8) | SECOND_EOP_CHAR;
    this->_payload.clear();
}



extern "C" {

    void *createPacketInstance(unsigned int id) {
        return new Packet(id);
    }
    int getType() {
        return PACKET_MODULE;
    }
}
