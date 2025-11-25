/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Packet
*/

#ifndef PACKET_HPP_
#define PACKET_HPP_

#include <memory>

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

        std::vector<uint8_t> packHeaderPacket(unsigned int idClient) override;
        std::vector<uint8_t> packPacket(std::vector<std::string> payload) override;
        bool unpackPacket(std::vector<uint8_t> data) override;

    protected:
    private:
        uint8_t _magicNumber;
        size_t _sequenceNumber;
        uint8_t _type;
        size_t _length;
        std::vector<std::string> _payload;
        short _endOfPacket;
        std::shared_ptr<ISerializer> _serializer;
};


#endif /* !PACKET_HPP_ */
