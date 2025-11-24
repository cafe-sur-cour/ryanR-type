/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** BigEndianSerialization
*/
#include "ISerializer.hpp"

#ifndef BIGENDIANSERIALIZATION_HPP_
#define BIGENDIANSERIALIZATION_HPP_

namespace net {

class BigEndianSerialization : public ISerializer {
    public:
        BigEndianSerialization();
        ~BigEndianSerialization() override;

        std::vector<uint8_t> serialize(const void* data, size_t size) override;
        bool deserialize(const std::vector<uint8_t>& data, void* output, size_t expectedSize) override;
    protected:
    private:
};

} // namespace net

#endif /* !BIGENDIANSERIALIZATION_HPP_ */
