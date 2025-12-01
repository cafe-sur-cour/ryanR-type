/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ISerializer
*/

#include <vector>
#include <cstdint>

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

        virtual uint64_t deserializeUInt(std::vector<uint8_t>) = 0;
        virtual uint64_t deserializeULong(std::vector<uint8_t>) = 0;
        virtual uint64_t deserializeUShort(std::vector<uint8_t>) = 0;
        virtual uint64_t deserializeUChar(std::vector<uint8_t>) = 0;
    protected:
    private:
};
} // namespace pm

#endif /* !ISERIALIZER_HPP_ */
