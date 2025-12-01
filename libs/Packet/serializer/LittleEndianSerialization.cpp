/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LittleEndianSerialization
*/

#include <vector>
#include "LittleEndianSerialization.hpp"

pm::LittleEndianSerialization::LittleEndianSerialization() {
}

pm::LittleEndianSerialization::~LittleEndianSerialization() {
}

std::vector<std::uint8_t> pm::LittleEndianSerialization::serializeUInt(
    uint64_t value) {
    std::vector<std::uint8_t> bytes;

    bytes.push_back(static_cast<std::uint8_t>(value & 0xFF));
    bytes.push_back(static_cast<std::uint8_t>((value >> 8) & 0xFF));
    bytes.push_back(static_cast<std::uint8_t>((value >> 16) & 0xFF));
    bytes.push_back(static_cast<std::uint8_t>((value >> 24) & 0xFF));
    return bytes;
}

std::vector<std::uint8_t> pm::LittleEndianSerialization::serializeULong(
    uint64_t value) {
    std::vector<std::uint8_t> bytes;

    bytes.push_back(static_cast<std::uint8_t>(value & 0xFF));
    bytes.push_back(static_cast<std::uint8_t>((value >> 8) & 0xFF));
    bytes.push_back(static_cast<std::uint8_t>((value >> 16) & 0xFF));
    bytes.push_back(static_cast<std::uint8_t>((value >> 24) & 0xFF));
    bytes.push_back(static_cast<std::uint8_t>((value >> 32) & 0xFF));
    bytes.push_back(static_cast<std::uint8_t>((value >> 40) & 0xFF));
    bytes.push_back(static_cast<std::uint8_t>((value >> 48) & 0xFF));
    bytes.push_back(static_cast<std::uint8_t>((value >> 56) & 0xFF));
    return bytes;
}

std::vector<std::uint8_t> pm::LittleEndianSerialization::serializeUShort(
    uint64_t value) {
    std::vector<std::uint8_t> bytes;

    bytes.push_back(static_cast<std::uint8_t>(value & 0xFF));
    bytes.push_back(static_cast<std::uint8_t>((value >> 8) & 0xFF));
    return bytes;
}

std::vector<std::uint8_t> pm::LittleEndianSerialization::serializeUChar(
    uint64_t value) {
    std::vector<std::uint8_t> bytes;
    bytes.push_back(static_cast<std::uint8_t>(value));
    return bytes;
}

uint64_t pm::LittleEndianSerialization::deserializeUInt(
    std::vector<uint8_t> data) {
    if (data.size() < 4) {
        return 0;
    }

    uint64_t value = (static_cast<uint64_t>(data[3]) << 24) |
        (static_cast<uint64_t>(data[2]) << 16) |
        (static_cast<uint64_t>(data[1]) << 8)  |
        (static_cast<uint64_t>(data[0]));

    return value;
}

uint64_t pm::LittleEndianSerialization::deserializeULong(
    std::vector<uint8_t> data) {
    if (data.size() < 8) {
        return 0;
    }

    uint64_t value = (static_cast<uint64_t>(data[7]) << 56) |
        (static_cast<uint64_t>(data[6]) << 48) |
        (static_cast<uint64_t>(data[5]) << 40) |
        (static_cast<uint64_t>(data[4]) << 32) |
        (static_cast<uint64_t>(data[3]) << 24) |
        (static_cast<uint64_t>(data[2]) << 16) |
        (static_cast<uint64_t>(data[1]) << 8)  |
        (static_cast<uint64_t>(data[0]));

    return value;
}

uint64_t pm::LittleEndianSerialization::deserializeUShort(
    std::vector<std::uint8_t> data) {
    if (data.size() < 2) {
        return 0;
    }

    uint64_t value = (static_cast<uint64_t>(data[1]) << 8) |
        (static_cast<uint64_t>(data[0]));

    return value;
}

uint64_t pm::LittleEndianSerialization::deserializeUChar(
    std::vector<std::uint8_t> data) {
    if (data.empty()) {
        return 0;
    }
    return static_cast<uint64_t>(data[0]);
}
