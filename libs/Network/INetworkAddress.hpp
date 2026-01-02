/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** NetworkAddress
*/

#ifndef INETWORK_ADDRESS_HPP
#define INETWORK_ADDRESS_HPP

#include <string>
#include <memory>

namespace net {

class INetworkAddress {
    public:
        virtual bool isV4() const = 0;
        virtual bool isV6() const = 0;
        virtual std::string toString() const = 0;

        virtual ~INetworkAddress() = default;
        virtual INetworkAddress& operator=(const INetworkAddress& other) = 0;
        virtual std::shared_ptr<void> getInternalAddress() = 0;
        virtual std::shared_ptr<const void> getInternalAddress() const = 0;
        virtual void setFromInternal(std::shared_ptr<void> internalAddr) = 0;
};

} // namespace net

#endif // INETWORK_ADDRESS_HPP
