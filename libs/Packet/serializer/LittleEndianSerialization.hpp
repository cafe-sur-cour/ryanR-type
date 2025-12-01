/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LittleEndianSerialization
*/

#ifndef LITTLEENDIANSERIALIZATION_HPP_
#define LITTLEENDIANSERIALIZATION_HPP_

#include "ISerializer.hpp"

namespace pm {
class LittleEndianSerialization : public ISerializer {
    public:
        LittleEndianSerialization();
        ~LittleEndianSerialization() override;

        std::vector<uint8_t> serializeUInt(uint64_t value) override;
        std::vector<uint8_t> serializeULong(uint64_t value) override;
        std::vector<uint8_t> serializeUShort(uint64_t value) override;
        std::vector<uint8_t> serializeUChar(uint64_t value) override;

        uint64_t deserializeUInt(std::vector<uint8_t>) override;
        uint64_t deserializeULong(std::vector<uint8_t>) override;
        uint64_t deserializeUShort(std::vector<uint8_t>) override;
        uint64_t deserializeUChar(std::vector<uint8_t>) override;
    protected:
    private:
};
} // namespace pm

#endif /* !LITTLEENDIANSERIALIZATION_HPP_ */
