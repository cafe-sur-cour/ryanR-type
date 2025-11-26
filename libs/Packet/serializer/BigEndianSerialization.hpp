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

        std::vector<uint8_t> serializeInt(uint64_t value) override;
        std::vector<uint8_t> serializeLong(uint64_t value) override;
        std::vector<uint8_t> serializeShort(uint64_t value) override;
        std::vector<uint8_t> serializeChar(uint64_t value) override;

        uint64_t deserializeInt(std::vector<uint8_t>) override;
        uint64_t deserializeLong(std::vector<uint8_t>) override;
        uint64_t deserializeShort(std::vector<uint8_t>) override;
        uint64_t deserializeChar(std::vector<uint8_t>) override;
    protected:
    private:
};

#endif /* !BIGENDIANSERIALIZATION_HPP_ */
