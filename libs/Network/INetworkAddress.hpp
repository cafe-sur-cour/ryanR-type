/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** NetworkAddress - Abstraction pour les adresses IP
*/

#ifndef INETWORK_ADDRESS_HPP
#define INETWORK_ADDRESS_HPP

#include <string>

namespace net {

class INetworkAddress {
    public:
        virtual bool isV4() const = 0;
        virtual bool isV6() const = 0;
        virtual std::string toString() const = 0;

        virtual ~INetworkAddress() = default;
        virtual INetworkAddress& operator=(const INetworkAddress& other) = 0;
        virtual void* getInternalAddress() = 0;
        virtual const void* getInternalAddress() const = 0;
        virtual void setFromInternal(void* internalAddr) = 0;
};

} // namespace net

#endif // INETWORK_ADDRESS_HPP
