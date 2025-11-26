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


class ISerializer {
    public:
        virtual ~ISerializer() = default;

        virtual std::vector<uint8_t> serializeInt(uint32_t value) = 0;
        virtual std::vector<uint8_t> serializeLong(uint64_t value) = 0;
        virtual std::vector<uint8_t> serializeShort(uint16_t value) = 0;
        virtual std::vector<uint8_t> serializeChar(uint8_t value) = 0;

        virtual uint32_t deserializeInt(std::vector<uint8_t>) = 0;
        virtual uint64_t deserializeLong(std::vector<uint8_t>) = 0;
        virtual uint16_t deserializeShort(std::vector<uint8_t>) = 0;
        virtual uint8_t deserializeChar(std::vector<uint8_t>) = 0;
    protected:
    private:
};

#endif /* !ISERIALIZER_HPP_ */
