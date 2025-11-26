/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** IPacket
*/

#ifndef IPACKET_HPP_
#define IPACKET_HPP_

#include <vector>
#include <cstdint>
#include "../Buffer/IBuffer.hpp"

class IPacket {
    public:
        virtual ~IPacket() = default;

        virtual uint8_t getMagicNumber() const = 0;
        virtual size_t getLength() const = 0;
        virtual size_t getSequenceNumber() const = 0;
        virtual uint8_t getType() const = 0;
        virtual std::vector<std::uint8_t> getPayload() const = 0;

        virtual void setType(uint8_t type) = 0;
        virtual void setLength(size_t length) = 0;

        virtual std::vector<uint8_t> packHeaderPacket(int idClient, unsigned int sequenceNumber, uint8_t type) = 0;
        virtual std::vector<uint8_t> packBodyPacket(std::vector<uint8_t> payload) = 0;
        virtual bool unpackPacket(std::vector<uint8_t> data) = 0;
};


#endif /* !IPACKET_HPP_ */
