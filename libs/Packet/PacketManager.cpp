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
#include "../../common/translationToECS.hpp"

pm::PacketManager::PacketManager(uint32_t seqNumber) {
    this->_idClient = 0;
    this->_sequenceNumber = seqNumber;
    this->_type = NO_OP_PACKET;
    this->_length = 0;
    this->_payload = std::vector<uint64_t>();
    this->_serializer = std::make_shared<BigEndianSerialization>();
    if (!this->_serializer) {
        throw err::PacketError(
            "[Server] Failed to load serializer",
            err::PacketError::SERIALIZER_ATTRIBUTION_FAILED
        );
    }
}

pm::PacketManager::~PacketManager() {
    this->clearAllHandlers();
    if (this->_serializer) {
        this->_serializer.reset();
    }
}

void pm::PacketManager::registerBuilder(
    uint8_t type,
    std::function<std::vector<uint8_t>(std::vector<uint64_t>)> builder
) {
    this->_packetHandlers[type] = builder;
}

void pm::PacketManager::registerParser(
    uint8_t type,
    std::function<bool(const std::vector<uint8_t>)> parser
) {
    this->_packetReceived[type] = parser;
}

void pm::PacketManager::registerLength(
    uint8_t type,
    uint32_t length
) {
    this->_packetLengths[type] = length;
}

void pm::PacketManager::registerGameStatePackFunction(
    std::function<std::vector<uint8_t>(std::vector<uint64_t>,
    std::shared_ptr<unsigned int>)> func
) {
    this->_packGSFunction.push_back(func);
}

void pm::PacketManager::registerGameStateUnpackFunction(
    std::function<unsigned int (const std::vector<uint8_t>,
    unsigned int)> func
) {
    this->_unpackGSFunction.push_back(func);
}

void pm::PacketManager::registerLengthCombEntry(
    uint8_t compType,
    uint32_t compLength,
    uint64_t compSize
) {
    this->_lengthComb.push_back(std::make_tuple(compType, compLength, compSize));
}

void pm::PacketManager::clearAllHandlers() {
    this->_packetHandlers.clear();
    this->_packetReceived.clear();
    this->_packetLengths.clear();
    this->_packGSFunction.clear();
    this->_unpackGSFunction.clear();
    this->_lengthComb.clear();
}

uint32_t pm::PacketManager::getLength() const {
    return this->_length;
}

uint32_t pm::PacketManager::getSequenceNumber() const {
    return this->_sequenceNumber;
}

uint8_t pm::PacketManager::getType() const {
    return this->_type;
}

std::vector<uint64_t> pm::PacketManager::getPayload() const {
    return this->_payload;
}

std::vector<std::vector<uint64_t>> pm::PacketManager::getBatchedPayloads() const {
    return this->_batchedPayloads;
}

uint8_t pm::PacketManager::getIdClient() const {
    return this->_idClient;
}

void pm::PacketManager::setType(uint8_t type) {
    this->_type = type;
}

void pm::PacketManager::setLength(uint32_t length) {
    this->_length = length;
}

void pm::PacketManager::setSequenceNumber(uint32_t sequenceNumber) {
    this->_sequenceNumber = sequenceNumber;
}

void pm::PacketManager::setPayload(std::vector<uint64_t> payload) {
    this->_payload = payload;
}

void pm::PacketManager::setIdClient(uint8_t idClient) {
    this->_idClient = idClient;
}

void pm::PacketManager::reset() {
    this->_idClient = 0;
    this->_sequenceNumber = 0;
    this->_type = NO_OP_PACKET;
    this->_length = 0;
    this->_payload.clear();
}

std::vector<uint64_t> pm::PacketManager::formatString(const std::string str) {
    if (str.size() > 8) {
        throw err::PacketError(
            "[PacketManager] String length exceeds 8 characters",
            err::PacketError::STRING_FORMATTING_ERROR
        );
    }
    int i = 0;
    std::vector<uint64_t> formatted;
    for (char c : str) {
        formatted.push_back(static_cast<uint64_t>(c));
        i++;
    }
    for (; i < 8; i++) {
        formatted.push_back('\0');
    }
    return formatted;
}



extern "C" {

    void *createPacketInstance(unsigned int id) {
        return new pm::PacketManager(id);
    }
    int getType() {
        return PACKET_MODULE;
    }
}
