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
#include "PacketManager.hpp"
#include "serializer/BigEndianSerialization.hpp"
#include "../../common/DLLoader/LoaderType.hpp"
#include "../../common/Error/PacketError.hpp"

PacketManager::PacketManager(uint32_t seqNumber) {
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
            err::PacketError::SERIALIZER_ATTRIBUTION_FAILED);
    }

    this->_packetHandlers = {
        {CONNECTION_CLIENT_PACKET, std::bind(&PacketManager::buildConnectionPacket,
            this, std::placeholders::_1)},
        {ACCEPTATION_PACKET, std::bind(&PacketManager::buildAcceptationPacket,
            this, std::placeholders::_1)},
        {DISCONNECTION_PACKET, std::bind(&PacketManager::buildDisconnectionPacket,
            this, std::placeholders::_1)},
        {EVENT_PACKET, std::bind(&PacketManager::buildEventPacket,
            this, std::placeholders::_1)}
    };

    this->_packetReceived = {
        {CONNECTION_CLIENT_PACKET, std::bind(&PacketManager::parseConnectionPacket,
            this, std::placeholders::_1)},
        {ACCEPTATION_PACKET, std::bind(&PacketManager::parseAcceptationPacket,
            this, std::placeholders::_1)},
        {DISCONNECTION_PACKET, std::bind(&PacketManager::parseDisconnectionPacket,
            this, std::placeholders::_1)},
        {EVENT_PACKET, std::bind(&PacketManager::parseEventPacket,
            this, std::placeholders::_1)}
    };

    this->_packetLengths = {
        {CONNECTION_CLIENT_PACKET, LENGTH_CONNECTION_PACKET},
        {ACCEPTATION_PACKET, LENGTH_ACCEPTATION_PACKET},
        {DISCONNECTION_PACKET, LENGTH_DISCONNECTION_PACKET},
        {EVENT_PACKET, LENGTH_EVENT_PACKET}
    };
}

PacketManager::~PacketManager() {
    if (this->_serializer) {
        this->_serializer.reset();
    }
}



uint8_t PacketManager::getMagicNumber() const {
    return this->_magicNumber;
}

uint32_t PacketManager::getLength() const {
    return this->_length;
}

uint32_t PacketManager::getSequenceNumber() const {
    return this->_sequenceNumber;
}

uint8_t PacketManager::getType() const {
    return this->_type;
}

std::vector<uint64_t> PacketManager::getPayload() const {
    return this->_payload;
}

uint8_t PacketManager::getIdClient() const {
    return this->_idClient;
}



void PacketManager::setType(uint8_t type) {
    this->_type = type;
}

void PacketManager::setLength(uint32_t length) {
    this->_length = length;
}

void PacketManager::setSequenceNumber(uint32_t sequenceNumber) {
    this->_sequenceNumber = sequenceNumber;
}

void PacketManager::setPayload(std::vector<uint64_t> payload) {
    this->_payload = payload;
}

void PacketManager::setIdClient(uint8_t idClient) {
    this->_idClient = idClient;
}



void PacketManager::reset() {
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
        return new PacketManager(id);
    }
    int getType() {
        return PACKET_MODULE;
    }
}
