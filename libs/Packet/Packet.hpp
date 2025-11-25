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
