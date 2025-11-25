/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Packet
*/

#include <vector>
#include <memory>
#include <string>
#include "Packet.hpp"
#include "../../common/DLLoader/LoaderType.hpp"
#include "serializer/BigEndianSerialization.hpp"

Packet::Packet(int seqNumber) {
    this->_magicNumber = MAGIC_NUMBER;
    this->_sequenceNumber = seqNumber;
    this->_type = 0x00;
    this->_length = 0;
    this->_payload = std::vector<std::string>();
    this->_endOfPacket = ('\r' << 8) | '\n';
    this->_serializer = std::make_shared<BigEndianSerialization>();
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

std::vector<uint8_t> Packet::packHeaderPacket(unsigned int idClient) {
    std::vector<uint8_t> header;
    std::vector<uint8_t> temp;

    temp = this->_serializer->serializeChar(this->_magicNumber);
    header.insert(header.end(), temp.begin(), temp.end());

    temp = this->_serializer->serializeInt(idClient);
    header.insert(header.end(), temp.begin(), temp.end());

    temp = this->_serializer->serializeLong(this->_sequenceNumber);
    header.insert(header.end(), temp.begin(), temp.end());

    temp = this->_serializer->serializeChar(this->_type);
    header.insert(header.end(), temp.begin(), temp.end());

    temp = this->_serializer->serializeLong(this->_length);
    header.insert(header.end(), temp.begin(), temp.end());

    temp = this->_serializer->serializeShort(this->_endOfPacket);
    header.insert(header.end(), temp.begin(), temp.end());
    return header;
}

std::vector<uint8_t> Packet::packPacket(std::vector<std::string> payload) {
    return std::vector<uint8_t>();
}

bool Packet::unpackPacket(std::vector<uint8_t> data) {
    return false;
}


extern "C" {

    int getType() {
        return PACKET_MODULE;
    }
}
