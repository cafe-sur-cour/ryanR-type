/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Varint - Variable-length integer encoding implementation
*/

#include <utility>
#include <vector>
#include "Varint.hpp"

std::vector<uint8_t> pm::Varint::encode(uint64_t value) {
    std::vector<uint8_t> result;
    result.reserve(encodedSize(value));

    do {
        uint8_t byte = static_cast<uint8_t>(value & DATA_MASK);
        value >>= DATA_BITS;

        if (value > 0) {
            byte |= CONTINUATION_BIT;
        }
        result.push_back(byte);
    } while (value > 0);

    return result;
}

std::pair<uint64_t, size_t> pm::Varint::decode(const std::vector<uint8_t> &data,
    size_t offset) {
    uint64_t result = 0;
    size_t shift = 0;
    size_t bytesRead = 0;

    while (offset + bytesRead < data.size() && bytesRead < MAX_VARINT_BYTES) {
        uint8_t byte = data[offset + bytesRead];
        bytesRead++;
        result |= static_cast<uint64_t>(byte & DATA_MASK) << shift;
        shift += DATA_BITS;

        if ((byte & CONTINUATION_BIT) == 0) {
            break;
        }
    }

    return {result, bytesRead};
}

std::vector<uint8_t> pm::Varint::encodeSigned(int64_t value) {
    uint64_t zigzag = static_cast<uint64_t>((value << 1) ^ (value >> 63));
    return encode(zigzag);
}

std::pair<int64_t, size_t> pm::Varint::decodeSigned(const std::vector<uint8_t> &data,
    size_t offset) {
    auto [zigzag, bytesRead] = decode(data, offset);
    int64_t result = static_cast<int64_t>((zigzag >> 1)
        ^ -(static_cast<int64_t>(zigzag) & 1));
    return {result, bytesRead};
}

size_t pm::Varint::encodedSize(uint64_t value) {
    if (value == 0) return 1;

    size_t size = 0;
    while (value > 0) {
        size++;
        value >>= DATA_BITS;
    }
    return size;
}
