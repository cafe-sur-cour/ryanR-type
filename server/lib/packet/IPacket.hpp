/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** IPacket
*/

#include "../buffer/IBuffer.hpp"

#ifndef IPACKET_HPP_
#define IPACKET_HPP_

namespace net {

class IPacket {
    public:
        virtual ~IPacket() {}

        virtual bool packPacket(const IBuffer &buffer) = 0;
        virtual bool unpackPacket(const IBuffer &buffer) = 0;
    protected:
    private:
};

} // namespace net

#endif /* !IPACKET_HPP_ */
