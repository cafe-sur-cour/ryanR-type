/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Packet
*/

#include "Packet.hpp"
#include "../../../common/LoaderType.hpp"


Packet::Packet(int idClient) {
    this->_magicNumber = 0x93;
    this->_idClient = idClient;
    this->_sequenceNumber = 0;
    this->_buffer = nullptr;
    this->_serializer = nullptr;
}

Packet::~Packet() {
    if (this->_buffer) {
        this->_buffer->deleteBuffer();
    }
    if (this->_serializer) {
        this->_serializer.reset();
    }

    this->_buffer = nullptr;
    this->_serializer = nullptr;
}

uint8_t Packet::getMagicNumber() const {
    return this->_magicNumber;
}

int Packet::getIdClient() const {
    return this->_idClient;
}

size_t Packet::getSequenceNumber() const {
    return this->_sequenceNumber;
}

void Packet::incrementSequenceNumber() {
    this->_sequenceNumber++;
}

bool Packet::packPacket(IBuffer &buffer) {
    return false;
}

bool Packet::unpackPacket(IBuffer &buffer) {
    return false;
}

extern "C" {

    int getType() {
        return PACKET_MODULE;
    }
}
