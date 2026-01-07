/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** BigEndianSerialization
*/

#ifndef BIGENDIANSERIALIZATION_HPP_
#define BIGENDIANSERIALIZATION_HPP_

#include "ISerializer.hpp"

namespace pm {
class BigEndianSerialization : public ISerializer {
    public:
        BigEndianSerialization();
        ~BigEndianSerialization() override;

        std::vector<uint8_t> serializeUInt(uint64_t value) override;
        std::vector<uint8_t> serializeULong(uint64_t value) override;
        std::vector<uint8_t> serializeUShort(uint64_t value) override;
        std::vector<uint8_t> serializeUChar(uint64_t value) override;
        std::vector<uint8_t> serializeUDouble(uint64_t value) override;

        std::vector<uint8_t> serializeVarint(uint64_t value) override;
        std::vector<uint8_t> serializeSignedVarint(int64_t value) override;

        uint64_t deserializeUInt(std::vector<uint8_t>) override;
        uint64_t deserializeULong(std::vector<uint8_t>) override;
        uint64_t deserializeUShort(std::vector<uint8_t>) override;
        uint64_t deserializeUChar(std::vector<uint8_t>) override;

        std::pair<uint64_t, size_t> deserializeVarint(const std::vector<uint8_t>& data, size_t offset = 0) override;
        std::pair<int64_t, size_t> deserializeSignedVarint(const std::vector<uint8_t>& data, size_t offset = 0) override;
    protected:
    private:
};
} // namespace pm

#endif /* !BIGENDIANSERIALIZATION_HPP_ */
