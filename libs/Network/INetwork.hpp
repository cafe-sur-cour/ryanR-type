/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** INetwork
*/

#ifndef INETWORK_HPP_
#define INETWORK_HPP_

#include "packet/IPacket.hpp"
#include "buffer/IBuffer.hpp"

namespace net {

class INetwork {
    public:

        virtual ~INetwork() = 0;

        virtual void init() = 0;
        virtual void stop() = 0;
        virtual void sendData(const IPacket &data, size_t size) = 0;
        virtual IPacket &receiveData(const IBuffer &buffer, size_t size) const = 0;

    protected:
    private:
};

} // namespace net

#endif /* !INETWORK_HPP_ */
