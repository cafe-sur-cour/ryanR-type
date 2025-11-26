/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Packet
*/

#ifndef PACKET_HPP_
#define PACKET_HPP_

#include <memory>
#include <map>
#include <vector>
#include <functional>

#include "IPacket.hpp"
#include "../Buffer/IBuffer.hpp"
#include "serializer/ISerializer.hpp"

#define MAGIC_NUMBER 0x93
#define FIRST_EOP_CHAR '\r'
#define SECOND_EOP_CHAR '\n'
#define HEADER_SIZE 13

#define LENGTH_CONNECTION_PACKET 11
#define LENGTH_ACCEPTATION_PACKET 4
#define LENGTH_DISCONNECTION_PACKET 4
#define LENGTH_EVENT_PACKET 5
#define LENGTH_EO_PACKET 2

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

class Packet : public IPacket {
    public:
        Packet(uint32_t seqNumber);
        ~Packet() override;

        uint8_t getMagicNumber() const override;
        uint32_t getLength() const override;
        uint32_t getSequenceNumber() const override;
        uint8_t getType() const override;
        std::vector<uint64_t> getPayload() const override;

        void setType(uint8_t type) override;
        void setLength(uint32_t length) override;
        void setSequenceNumber(uint32_t sequenceNumber) override;
        void setPayload(std::vector<uint64_t> payload) override;
        void setIdClient(uint8_t idClient) override;

        std::vector<uint8_t> packHeaderPacket(uint8_t idClient, uint32_t sequenceNumber, uint8_t type) override;
        std::vector<uint8_t> packBodyPacket(std::vector<uint64_t> payload) override;
        bool unpack(std::vector<uint8_t> data) override;

        void reset() override;
    private:
        uint8_t _magicNumber;
        uint8_t _idClient;
        uint32_t _sequenceNumber;
        uint8_t _type;
        uint32_t _length;
        uint16_t _endOfPacket;
        std::vector<uint64_t> _payload;
        std::shared_ptr<ISerializer> _serializer;
        std::map<uint8_t, std::function<std::vector<uint8_t>(std::vector<uint64_t>)>> _packetHandlers;
        std::map<uint8_t, std::function<bool(const std::vector<uint8_t>)>> _packetReceived;
        std::map<uint8_t, uint32_t> _packetLengths;

        bool unpackHeader(const std::vector<uint8_t> data);
        bool unpackBody(const std::vector<uint8_t> data);

        std::vector<uint8_t> buildConnectionPacket(std::vector<uint64_t> payload);
        bool parseConnectionPacket(const std::vector<uint8_t> payload);
        std::vector<uint8_t> buildAcceptationPacket(std::vector<uint64_t> payload);
        bool parseAcceptationPacket(const std::vector<uint8_t> payload);
        std::vector<uint8_t> buildDisconnectionPacket(std::vector<uint64_t> payload);
        bool parseDisconnectionPacket(const std::vector<uint8_t> payload);
        std::vector<uint8_t> buildEventPacket(std::vector<uint64_t> payload);
        bool parseEventPacket(const std::vector<uint8_t> payload);
};

extern "C" {
    void *createPacketInstance(int id);
    int getType();
}
#endif /* !PACKET_HPP_ */
