/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** BigEndianSerialization
*/

#include <vector>
#include "BigEndianSerialization.hpp"

pm::BigEndianSerialization::BigEndianSerialization() {
}

pm::BigEndianSerialization::~BigEndianSerialization() {
}

std::vector<std::uint8_t> pm::BigEndianSerialization::serializeUInt(
    uint64_t value) {
    std::vector<std::uint8_t> bytes;

    bytes.push_back(static_cast<std::uint8_t>((value >> 24) & 0xFF));
    bytes.push_back(static_cast<std::uint8_t>((value >> 16) & 0xFF));
    bytes.push_back(static_cast<std::uint8_t>((value >> 8) & 0xFF));
    bytes.push_back(static_cast<std::uint8_t>(value & 0xFF));
    return bytes;
}

std::vector<std::uint8_t> pm::BigEndianSerialization::serializeULong(
    uint64_t value) {
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

std::vector<std::uint8_t> pm::BigEndianSerialization::serializeUShort(
    uint64_t value) {
    std::vector<std::uint8_t> bytes;

    bytes.push_back(static_cast<std::uint8_t>((value >> 8) & 0xFF));
    bytes.push_back(static_cast<std::uint8_t>(value & 0xFF));
    return bytes;
}

std::vector<std::uint8_t> pm::BigEndianSerialization::serializeUChar(
    uint64_t value) {
    std::vector<std::uint8_t> bytes;
    bytes.push_back(static_cast<std::uint8_t>(value));
    return bytes;
}

std::vector<std::uint8_t> pm::BigEndianSerialization::serializeUDouble(
    uint64_t value) {
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

uint64_t pm::BigEndianSerialization::deserializeUInt(
    std::vector<uint8_t> data) {
    if (data.size() < 4) {
        return 0;
    }

    uint64_t value = (static_cast<uint64_t>(data[0]) << 24) |
        (static_cast<uint64_t>(data[1]) << 16) |
        (static_cast<uint64_t>(data[2]) << 8)  |
        (static_cast<uint64_t>(data[3]));

    return value;
}

uint64_t pm::BigEndianSerialization::deserializeULong(
    std::vector<uint8_t> data) {
    if (data.size() < 8) {
        return 0;
    }

    uint64_t value = (static_cast<uint64_t>(data[0]) << 56) |
        (static_cast<uint64_t>(data[1]) << 48) |
        (static_cast<uint64_t>(data[2]) << 40) |
        (static_cast<uint64_t>(data[3]) << 32) |
        (static_cast<uint64_t>(data[4]) << 24) |
        (static_cast<uint64_t>(data[5]) << 16) |
        (static_cast<uint64_t>(data[6]) << 8)  |
        (static_cast<uint64_t>(data[7]));

    return value;
}

uint64_t pm::BigEndianSerialization::deserializeUShort(
    std::vector<std::uint8_t> data) {
    if (data.size() < 2) {
        return 0;
    }

    uint64_t value = (static_cast<uint64_t>(data[0]) << 8) |
        (static_cast<uint64_t>(data[1]));

    return value;
}

uint64_t pm::BigEndianSerialization::deserializeUChar(
    std::vector<std::uint8_t> data) {
    if (data.empty()) {
        return 0;
    }
    return static_cast<uint64_t>(data[0]);
}
