/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Varint - Variable-length integer encoding utility
*/

#ifndef VARINT_HPP_
#define VARINT_HPP_

#include <vector>
#include <cstdint>
#include <utility>

namespace pm {


class Varint {
    public:
        static std::vector<uint8_t> encode(uint64_t value);
        static std::pair<uint64_t, size_t> decode(const std::vector<uint8_t> &data, size_t offset = 0);
        static std::vector<uint8_t> encodeSigned(int64_t value);
        static std::pair<int64_t, size_t> decodeSigned(const std::vector<uint8_t> &data, size_t offset = 0);
        static size_t encodedSize(uint64_t value);
        static constexpr size_t MAX_VARINT_BYTES = 10;

    private:
        static constexpr uint8_t CONTINUATION_BIT = 0x80;
        static constexpr uint8_t DATA_MASK = 0x7F;
        static constexpr uint8_t DATA_BITS = 7;
};

} // namespace pm

#endif /* !VARINT_HPP_ */
