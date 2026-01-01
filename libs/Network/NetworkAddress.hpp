/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** NetworkAddress - Abstraction pour les adresses IP
*/

#ifndef NETWORK_ADDRESS_HPP
#define NETWORK_ADDRESS_HPP

#include <string>

namespace net {

class NetworkAddress {
public:
    static NetworkAddress fromString(const std::string& address);
    static NetworkAddress anyV4();
    static NetworkAddress anyV6();
    static NetworkAddress loopbackV4();
    static NetworkAddress loopbackV6();
    bool isV4() const;
    bool isV6() const;
    std::string toString() const;

    NetworkAddress();
    ~NetworkAddress();
    NetworkAddress(const NetworkAddress& other);
    NetworkAddress& operator=(const NetworkAddress& other);

    void* getInternalAddress();
    const void* getInternalAddress() const;
    void setFromInternal(void* internalAddr);

private:
    class Impl;
    Impl* _impl;
};

} // namespace net

#endif // NETWORK_ADDRESS_HPP
