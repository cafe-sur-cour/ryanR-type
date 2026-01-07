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
#include <functional>
#include <string>
#include "IBuffer.hpp"

#define MAGIC_NUMBER 0x93
#define HEADER_SIZE 11

#define LENGTH_CONNECTION_PACKET 8
#define LENGTH_ACCEPTATION_PACKET 1
#define LENGTH_DISCONNECTION_PACKET 1
#define LENGTH_EVENT_PACKET 9
#define LENGTH_END_GAME_PACKET 0
#define LENGTH_DEATH_PACKET 8
#define LENGTH_WHOAMI_PACKET 0
#define LENGTH_SERVER_STATUS_PACKET 32
#define LENGTH_LOBBY_CODE_PACKET 8
#define LENGTH_REQUEST_LOBBY_PACKET 0
#define LENGTH_CONNECT_TO_LOBBY_PACKET 1
#define LENGTH_REGISTER_PACKET 16
#define LENGTH_CONNECT_USER_PACKET 8
#define LENGTH_LOGIN_PACKET 16

#define NO_OP_PACKET 0x00
#define CONNECTION_CLIENT_PACKET 0x01
#define ACCEPTATION_PACKET 0x02
#define DISCONNECTION_PACKET 0x03
#define EVENT_PACKET 0x04
#define GAME_STATE_PACKET 0x05
#define END_GAME_PACKET 0x06
#define CAN_START_PACKET 0x07
#define CLIENT_READY_PACKET 0x08
#define SPAWN_PLAYER_PACKET 0x09
#define DEATH_PLAYER_PACKET 0x0A
#define WHOAMI_PACKET 0x0B
#define SERVER_STATUS_PACKET 0x0C
#define REQUEST_LOBBY_PACKET 0x0D
#define SEND_LOBBY_CODE_PACKET 0x0E
#define CONNECT_TO_LOBBY 0x0F
#define LOBBY_MASTER_REQUEST_START 0x10
#define LOBBY_CONNECT_VALUE 0x11
#define LEVEL_COMPLETE_PACKET 0x12
#define NEXT_LEVEL_PACKET 0x13
#define REGISTER_PACKET 0x14
#define CONNECT_USER_PACKET 0x15
#define LOGIN_PACKET 0x16

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

        virtual void registerBuilder(uint8_t type, std::function<std::vector<uint8_t>(std::vector<uint64_t>)> builder) = 0;
        virtual void registerParser(uint8_t type, std::function<bool(const std::vector<uint8_t>)> parser) = 0;
        virtual void registerLength(uint8_t type, uint32_t length) = 0;
        virtual void registerGameStatePackFunction(std::function<std::vector<uint8_t>(std::vector<uint64_t>, std::shared_ptr<unsigned int>)> func) = 0;
        virtual void registerGameStateUnpackFunction(std::function<unsigned int(const std::vector<uint8_t>, unsigned int)> func) = 0;
        virtual void registerLengthCombEntry(uint8_t compType, uint32_t compLength, uint64_t compSize) = 0;
        virtual void clearAllHandlers() = 0;
};
} // namespace pm

#endif /* !IPacketManager_HPP_ */
