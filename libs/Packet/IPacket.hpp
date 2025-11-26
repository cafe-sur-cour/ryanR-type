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
        virtual uint32_t getLength() const = 0;
        virtual uint32_t getSequenceNumber() const = 0;
        virtual uint8_t getType() const = 0;
        virtual std::vector<uint64_t> getPayload() const = 0;

        virtual void setType(uint8_t type) = 0;
        virtual void setLength(uint32_t length) = 0;
        virtual void setSequenceNumber(uint32_t sequenceNumber) = 0;
        virtual void setPayload(std::vector<uint64_t> payload) = 0;
        virtual void setIdClient(uint8_t idClient) = 0;

        virtual std::vector<uint8_t> packHeaderPacket(uint8_t idClient, unsigned int sequenceNumber, uint8_t type) = 0;
        virtual std::vector<uint8_t> packBodyPacket(std::vector<uint64_t> payload) = 0;
        virtual bool unpackPacket(std::vector<uint8_t> data) = 0;

        virtual void resetPacket() = 0;
};


#endif /* !IPACKET_HPP_ */
