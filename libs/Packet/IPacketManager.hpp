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

namespace pm {
class IPacketManager {
    public:
        virtual ~IPacketManager() = default;

        virtual uint8_t getMagicNumber() const = 0;
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
        virtual std::vector<uint8_t> pack(uint8_t idClient, uint32_t sequenceNumber, uint8_t type) = 0;
        virtual std::vector<uint8_t> pack(std::vector<uint64_t> payload) = 0;
        virtual bool unpack(std::vector<uint8_t> data) = 0;

        virtual void reset() = 0;
};
} // namespace pm

#endif /* !IPacketManager_HPP_ */
