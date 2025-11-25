/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** BigEndianSerialization
*/
#include "ISerializer.hpp"

#ifndef BIGENDIANSERIALIZATION_HPP_
#define BIGENDIANSERIALIZATION_HPP_

class BigEndianSerialization : public ISerializer {
    public:
        BigEndianSerialization();
        ~BigEndianSerialization() override;

        std::vector<std::uint8_t> serializeInt(int value) override;
        std::vector<std::uint8_t> serializeLong(int64_t value) override;
        std::vector<std::uint8_t> serializeShort(uint16_t value) override;
        std::vector<std::uint8_t> serializeChar(uint8_t value) override;
        bool deserialize(const std::vector<uint8_t>& data, void* output, size_t expectedSize) override;
    protected:
    private:
};

#endif /* !BIGENDIANSERIALIZATION_HPP_ */
