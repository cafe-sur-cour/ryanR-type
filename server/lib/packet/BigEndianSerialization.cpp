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

std::vector<uint8_t> BigEndianSerialization::serialize(const void* data,
    size_t size) {
    return std::vector<uint8_t>();
}

bool BigEndianSerialization::deserialize(const std::vector<uint8_t>& data,
    void* output, size_t expectedSize) {
    return false;
}
