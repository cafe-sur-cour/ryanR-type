/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Compression utility using zlib
*/

#ifndef COMPRESSION_HPP_
#define COMPRESSION_HPP_

#include <vector>
#include <cstdint>

namespace pm {

class Compression {
    public:
        static std::vector<uint8_t> compress(const std::vector<uint8_t>& data, int level = 6);

        static std::vector<uint8_t> decompress(const std::vector<uint8_t>& compressedData, size_t originalSize = 0);
        static bool shouldCompress(size_t dataSize);
        static constexpr size_t getCompressionOverhead() { return 4; }

    private:
        static constexpr size_t MIN_COMPRESS_SIZE = 50;
        static constexpr size_t MAX_DECOMPRESS_SIZE = 65536;
};

} // namespace pm

#endif /* !COMPRESSION_HPP_ */
