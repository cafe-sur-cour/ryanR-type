/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** IPacket
*/

#ifndef IPACKET_HPP_
#define IPACKET_HPP_

class IPacket {
    public:
        virtual ~IPacket() = 0;

        virtual bool packPacket(IBuffer &buffer) = 0;
        virtual bool unpackPacket(IBuffer &buffer) = 0;
    protected:
    private:
};

#endif /* !IPACKET_HPP_ */
