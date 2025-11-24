/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** IPacket
*/

#ifndef IPACKET_HPP_
#define IPACKET_HPP_

#include "../Buffer/IBuffer.hpp"

class IPacket {
    public:
        virtual ~IPacket() {}

        virtual bool packPacket(const IBuffer &buffer) = 0;
        virtual bool unpackPacket(const IBuffer &buffer) = 0;
    protected:
    private:
};


#endif /* !IPACKET_HPP_ */
