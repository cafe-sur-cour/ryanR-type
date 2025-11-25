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

#define LENGTH_CONNECTION_PACKET 11
#define LENGTH_DISCONNECTION_PACKET 7
#define LENGTH_EVENT_PACKET 8
#define LENGTH_EO_PACKET 2

#define NO_OP_PACKET 0x00
#define CONNECTION_CLIENT_PACKET 0x01
#define CONNECTION_SERVER_PACKET 0x02
#define DISCONNECTION_PACKET 0x03
#define EVENT_PACKET 0x04
#define GAME_STATE_PACKET 0x05
#define MAP_SEND_PACKET 0x06
#define END_MAP_PACKET 0x07
#define END_GAME_PACKET 0x08
#define CAN_START_PACKET 0x09

class Packet : public IPacket {
    public:
        Packet(int idClient);
        ~Packet() override;

        uint8_t getMagicNumber() const override;
        size_t getLength() const override;
        size_t getSequenceNumber() const override;
        uint8_t getType() const override;

        void setType(uint8_t type) override;
        void setLength(size_t length) override;

        std::vector<uint8_t> packHeaderPacket(unsigned int idClient, unsigned int sequenceNumber, uint8_t type) override;
        std::vector<uint8_t> packBodyPacket(std::vector<std::uint8_t> payload) override;
        bool unpackPacket(std::vector<uint8_t> data) override;

        std::vector<std::uint8_t> connectionPacket(std::vector<std::uint8_t> payload);
        std::vector<std::uint8_t> disconnectionPacket(std::vector<std::uint8_t> payload);
        std::vector<std::uint8_t> eventPacket(std::vector<std::uint8_t> payload);

    private:
        uint8_t _magicNumber;
        unsigned int _sequenceNumber;
        uint8_t _type;
        unsigned int _length;
        short _endOfPacket;
        std::shared_ptr<ISerializer> _serializer;
        std::map<uint8_t, std::function<std::vector<std::uint8_t>(std::vector<std::uint8_t>)>> _packetHandlers;
        std::map<uint8_t, unsigned int> _packetLengths;
};


#endif /* !PACKET_HPP_ */
