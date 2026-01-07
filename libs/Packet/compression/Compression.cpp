/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Compression utility implementation using zlib
*/

#include <zlib.h>

#include <vector>
#include <iostream>
#include "Compression.hpp"

std::vector<uint8_t> pm::Compression::compress(const std::vector<uint8_t>& data, int level) {
    if (data.empty()) {
        return {};
    }

    if (!shouldCompress(data.size())) {
        return {};
    }

    if (level < 1) level = 1;
    if (level > 9) level = 9;

    uLongf compressedSize = compressBound(static_cast<uLong>(data.size()));

    std::vector<uint8_t> compressed(getCompressionOverhead() + compressedSize);

    uint32_t originalSize = static_cast<uint32_t>(data.size());
    compressed[0] = static_cast<uint8_t>(originalSize & 0xFF);
    compressed[1] = static_cast<uint8_t>((originalSize >> 8) & 0xFF);
    compressed[2] = static_cast<uint8_t>((originalSize >> 16) & 0xFF);
    compressed[3] = static_cast<uint8_t>((originalSize >> 24) & 0xFF);

    int result = compress2(
        compressed.data() + getCompressionOverhead(),
        &compressedSize,
        data.data(),
        static_cast<uLong>(data.size()),
        level
    );

    if (result != Z_OK) {
        std::cerr << "[COMPRESSION] compress2 failed with error code: "
            << result << std::endl;
        return {};
    }
    compressed.resize(getCompressionOverhead() + compressedSize);
    if (compressed.size() >= data.size()) {
        return {};
    }

    return compressed;
}

std::vector<uint8_t> pm::Compression::decompress(const std::vector<uint8_t>
    &compressedData, size_t originalSize) {
    if (compressedData.size() < getCompressionOverhead()) {
        std::cerr << "[COMPRESSION] Compressed data too small to contain header"
            << std::endl;
        return {};
    }

    uint32_t storedSize =
        static_cast<uint32_t>(compressedData[0]) |
        (static_cast<uint32_t>(compressedData[1]) << 8) |
        (static_cast<uint32_t>(compressedData[2]) << 16) |
        (static_cast<uint32_t>(compressedData[3]) << 24);

    if (originalSize == 0) {
        originalSize = storedSize;
    }

    if (originalSize > MAX_DECOMPRESS_SIZE) {
        std::cerr << "[COMPRESSION] Decompressed size too large: " <<
            originalSize << std::endl;
        return {};
    }

    std::vector<uint8_t> decompressed(originalSize);
    uLongf destLen = static_cast<uLongf>(originalSize);

    int result = uncompress(
        decompressed.data(),
        &destLen,
        compressedData.data() + getCompressionOverhead(),
        static_cast<uLong>(compressedData.size() - getCompressionOverhead())
    );

    if (result != Z_OK) {
        std::cerr << "[COMPRESSION] uncompress failed with error code: " <<
            result << std::endl;
        return {};
    }
    decompressed.resize(destLen);
    return decompressed;
}

bool pm::Compression::shouldCompress(size_t dataSize) {
    return dataSize >= MIN_COMPRESS_SIZE;
}
