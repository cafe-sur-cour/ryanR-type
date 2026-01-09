/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ISerializer
*/

#include <vector>
#include <cstdint>
#include <utility>

#ifndef ISERIALIZER_HPP_
#define ISERIALIZER_HPP_

namespace pm {
class ISerializer {
    public:
        virtual ~ISerializer() = default;

        virtual std::vector<uint8_t> serializeUInt(uint64_t value) = 0;
        virtual std::vector<uint8_t> serializeULong(uint64_t value) = 0;
        virtual std::vector<uint8_t> serializeUShort(uint64_t value) = 0;
        virtual std::vector<uint8_t> serializeUChar(uint64_t value) = 0;
        virtual std::vector<uint8_t> serializeUDouble(uint64_t value) = 0;

        virtual std::vector<uint8_t> serializeVarint(uint64_t value) = 0;
        virtual std::vector<uint8_t> serializeSignedVarint(int64_t value) = 0;

        virtual uint64_t deserializeUInt(std::vector<uint8_t>) = 0;
        virtual uint64_t deserializeULong(std::vector<uint8_t>) = 0;
        virtual uint64_t deserializeUShort(std::vector<uint8_t>) = 0;
        virtual uint64_t deserializeUChar(std::vector<uint8_t>) = 0;

        virtual std::pair<uint64_t, size_t> deserializeVarint(const std::vector<uint8_t>& data, size_t offset = 0) = 0;
        virtual std::pair<int64_t, size_t> deserializeSignedVarint(const std::vector<uint8_t>& data, size_t offset = 0) = 0;
    protected:
    private:
};
} // namespace pm

#endif /* !ISERIALIZER_HPP_ */
