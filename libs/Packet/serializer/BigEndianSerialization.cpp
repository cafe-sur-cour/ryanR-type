/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** BigEndianSerialization
*/

#include <vector>
#include "BigEndianSerialization.hpp"

BigEndianSerialization::BigEndianSerialization() {
}

BigEndianSerialization::~BigEndianSerialization() {
}

std::vector<std::uint8_t> BigEndianSerialization::serializeInt(int value) {
    std::vector<std::uint8_t> bytes;

    bytes.push_back(static_cast<std::uint8_t>((value >> 24) & 0xFF));
    bytes.push_back(static_cast<std::uint8_t>((value >> 16) & 0xFF));
    bytes.push_back(static_cast<std::uint8_t>((value >> 8) & 0xFF));
    bytes.push_back(static_cast<std::uint8_t>(value & 0xFF));
    return bytes;
}

std::vector<std::uint8_t> BigEndianSerialization::serializeLong(int64_t value) {
    std::vector<std::uint8_t> bytes;

    bytes.push_back(static_cast<std::uint8_t>((value >> 56) & 0xFF));
    bytes.push_back(static_cast<std::uint8_t>((value >> 48) & 0xFF));
    bytes.push_back(static_cast<std::uint8_t>((value >> 40) & 0xFF));
    bytes.push_back(static_cast<std::uint8_t>((value >> 32) & 0xFF));
    bytes.push_back(static_cast<std::uint8_t>((value >> 24) & 0xFF));
    bytes.push_back(static_cast<std::uint8_t>((value >> 16) & 0xFF));
    bytes.push_back(static_cast<std::uint8_t>((value >> 8) & 0xFF));
    bytes.push_back(static_cast<std::uint8_t>(value & 0xFF));
    return bytes;
}

std::vector<std::uint8_t> BigEndianSerialization::serializeShort(
    uint16_t value) {
    std::vector<std::uint8_t> bytes;

    bytes.push_back(static_cast<std::uint8_t>((value >> 8) & 0xFF));
    bytes.push_back(static_cast<std::uint8_t>(value & 0xFF));
    return bytes;
}

std::vector<std::uint8_t> BigEndianSerialization::serializeChar(uint8_t value) {
    std::vector<std::uint8_t> bytes;
    bytes.push_back(static_cast<std::uint8_t>(value));
    return bytes;
}

std::uint8_t BigEndianSerialization::deserializeInt(
    std::vector<std::uint8_t> data) {
    if (data.size() < 4) {
        return 0;
    }

    std::uint32_t value = (static_cast<std::uint32_t>(data[0]) << 24) |
        (static_cast<std::uint32_t>(data[1]) << 16) |
        (static_cast<std::uint32_t>(data[2]) << 8)  |
        (static_cast<std::uint32_t>(data[3]));

    return static_cast<std::uint8_t>(value);
}
