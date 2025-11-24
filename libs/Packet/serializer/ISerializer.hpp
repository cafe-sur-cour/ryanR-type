/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ISerializer
*/

#include <vector>
#include <cstdint>

#ifndef ISERIALIZER_HPP_
#define ISERIALIZER_HPP_

namespace net {

class ISerializer {
    public:
        virtual ~ISerializer();

        virtual std::vector<uint8_t> serialize(const void* data, size_t size) = 0;
        virtual bool deserialize(const std::vector<uint8_t>& data, void* output, size_t expectedSize) = 0;
    protected:
    private:
};

} // namespace net

#endif /* !ISERIALIZER_HPP_ */
