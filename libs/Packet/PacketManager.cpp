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

    this->_packetHandlers = {
        {
            static_cast<uint8_t>(CONNECTION_CLIENT_PACKET), std::bind(
                &pm::PacketManager::buildConnectionPacket,
                this, std::placeholders::_1)
        },
        {
            static_cast<uint8_t>(ACCEPTATION_PACKET), std::bind(
                &pm::PacketManager::buildAcceptationPacket,
                this, std::placeholders::_1)
        },
        {
            static_cast<uint8_t>(DISCONNECTION_PACKET), std::bind(
                &pm::PacketManager::buildDisconnectionPacket,
                this, std::placeholders::_1)
        },
        {
            static_cast<uint8_t>(EVENT_PACKET), std::bind(
            &pm::PacketManager::buildEventPacket,
            this, std::placeholders::_1)
        },
        {
            static_cast<uint8_t>(MAP_SEND_PACKET), std::bind(
            &pm::PacketManager::buildMapPacket,
            this, std::placeholders::_1)
        }
    };

    this->_packetReceived = {
        {
            static_cast<uint8_t>(CONNECTION_CLIENT_PACKET), std::bind(
                &pm::PacketManager::parseConnectionPacket,
                this, std::placeholders::_1)
        },
        {
            static_cast<uint8_t>(ACCEPTATION_PACKET), std::bind(
                &pm::PacketManager::parseAcceptationPacket,
                this, std::placeholders::_1),
        },
        {
            static_cast<uint8_t>(DISCONNECTION_PACKET), std::bind(
                &pm::PacketManager::parseDisconnectionPacket,
                this, std::placeholders::_1)
        },
        {
            static_cast<uint8_t>(EVENT_PACKET), std::bind(
                &pm::PacketManager::parseEventPacket,
                this, std::placeholders::_1)
        },
        {
            static_cast<uint8_t>(MAP_SEND_PACKET), std::bind(
            &pm::PacketManager::parseMapPacket,
            this, std::placeholders::_1)
        }
    };

    this->_packetLengths = {
        {
            static_cast<uint8_t>(CONNECTION_CLIENT_PACKET),
            LENGTH_CONNECTION_PACKET
        },
        {
            static_cast<uint8_t>(ACCEPTATION_PACKET),
            LENGTH_ACCEPTATION_PACKET
        },
        {
            static_cast<uint8_t>(DISCONNECTION_PACKET),
            LENGTH_DISCONNECTION_PACKET
        },
        {
            static_cast<uint8_t>(EVENT_PACKET),
            LENGTH_EVENT_PACKET
        }
    };
}

pm::PacketManager::~PacketManager() {
    if (this->_serializer) {
        this->_serializer.reset();
    }
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
