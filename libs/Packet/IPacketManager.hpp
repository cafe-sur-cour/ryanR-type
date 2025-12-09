/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** IPacketManager
*/

#ifndef IPacketManager_HPP_
#define IPacketManager_HPP_

#include <vector>
#include <cstdint>
#include <string>
#include "../Buffer/IBuffer.hpp"

#define MAGIC_NUMBER 0x93
#define HEADER_SIZE 11

#define LENGTH_CONNECTION_PACKET 8
#define LENGTH_ACCEPTATION_PACKET 1
#define LENGTH_DISCONNECTION_PACKET 1
#define LENGTH_EVENT_PACKET 17
#define LENGTH_END_GAME_PACKET 1

#define NO_OP_PACKET 0x00
#define CONNECTION_CLIENT_PACKET 0x01
#define ACCEPTATION_PACKET 0x02
#define DISCONNECTION_PACKET 0x03
#define EVENT_PACKET 0x04
#define GAME_STATE_PACKET 0x05
#define MAP_SEND_PACKET 0x06
#define END_MAP_PACKET 0x07
#define END_GAME_PACKET 0x08
#define CAN_START_PACKET 0x09

namespace pm {

    class IPacketManager {
    public:
        virtual ~IPacketManager() = default;

        virtual uint32_t getLength() const = 0;
        virtual uint32_t getSequenceNumber() const = 0;
        virtual uint8_t getType() const = 0;
        virtual std::vector<uint64_t> getPayload() const = 0;
        virtual uint8_t getIdClient() const = 0;

        virtual void setType(uint8_t type) = 0;
        virtual void setLength(uint32_t length) = 0;
        virtual void setSequenceNumber(uint32_t sequenceNumber) = 0;
        virtual void setPayload(std::vector<uint64_t> payload) = 0;
        virtual void setIdClient(uint8_t idClient) = 0;

        virtual std::vector<uint64_t> formatString(const std::string str) = 0;
        virtual std::vector<uint8_t> pack(uint8_t idClient, uint32_t sequenceNumber, uint8_t type, std::vector<uint64_t> payload) = 0;
        virtual bool unpack(std::vector<uint8_t> data) = 0;

        virtual void reset() = 0;
};
} // namespace pm

#endif /* !IPacketManager_HPP_ */
