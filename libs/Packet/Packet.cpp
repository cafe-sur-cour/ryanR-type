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

Packet::Packet(int seqNumber) {
    this->_magicNumber = MAGIC_NUMBER;
    this->_idClient = 0;
    this->_sequenceNumber = seqNumber;
    this->_type = NO_OP_PACKET;
    this->_length = 0;
    this->_endOfPacket = (FIRST_EOP_CHAR << 8) | SECOND_EOP_CHAR;
    this->_payload = std::vector<std::uint8_t>();
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

void Packet::setSequenceNumber(size_t sequenceNumber) {
    this->_sequenceNumber = sequenceNumber;
}

void Packet::setPayload(std::vector<std::uint8_t> payload) {
    this->_payload = payload;
}

void Packet::setIdClient(unsigned int idClient) {
    this->_idClient = idClient;
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
    if (data.empty()) {
        std::cerr << "[SERVER] Error: unpackPacket(): Empty packet data" << std::endl;
        return false;
    }

    if (data.at(0) == this->_magicNumber) {
        if (data.size() != HEADER_SIZE) {
            std::cerr << "[SERVER] Error: unpackPacket(): Invalid header data" << std::endl;
            return false;
        }
        this->_idClient = (data.at(1) << 24) | (data.at(2) << 16) |
            (data.at(3) << 8) | data.at(4);
        this->_sequenceNumber = (data.at(5) << 24) | (data.at(6) << 16) |
            (data.at(7) << 8) | data.at(8);
        this->_type = data.at(9);
        this->_length = (data.at(10) << 24) | (data.at(11) << 16) |
            (data.at(12) << 8) | data.at(13);
        if (data.at(14) != (FIRST_EOP_CHAR) ||
            data.at(15) != (SECOND_EOP_CHAR)) {
            std::cerr << "[SERVER] Error: unpackPacket(): Invalid end of packet characters" << std::endl;
            return false;
        }
        return true;
    } else {
        if (this->_type == data.at(0)) {
            for (auto &received : this->_packetReceived) {
                if (received.first == this->_type) {
                    return received.second(data);
                }
            }
            std::cerr << "[SERVER] Error: unpackPacket(): Unknown packet type received" << std::endl;
            return false;
        }
        std::cerr << "[SERVER] Error: unpackPacket(): Packet type mismatch" << std::endl;
        return false;
    }
}



void Packet::resetPacket() {
    this->_magicNumber = MAGIC_NUMBER;
    this->_idClient = 0;
    this->_sequenceNumber = 0;
    this->_type = NO_OP_PACKET;
    this->_length = 0;
    this->_endOfPacket = (FIRST_EOP_CHAR << 8) | SECOND_EOP_CHAR;
    this->_payload.clear();
}



extern "C" {

    void *createPacketInstance(int id) {
        return new Packet(id);
    }
    int getType() {
        return PACKET_MODULE;
    }
}
