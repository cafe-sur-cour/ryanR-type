/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** NetworkAddress implementation
*/

#include "NetworkAddress.hpp"
#include <asio.hpp>

namespace net {

class NetworkAddress::Impl {
    public:
        asio::ip::address address;
};

NetworkAddress::NetworkAddress() : _impl(new Impl()) {
}

NetworkAddress::~NetworkAddress() {
    delete _impl;
}

NetworkAddress::NetworkAddress(const NetworkAddress& other) : _impl(new Impl()) {
    _impl->address = other._impl->address;
}

NetworkAddress& NetworkAddress::operator=(const NetworkAddress& other) {
    if (this != &other) {
        _impl->address = other._impl->address;
    }
    return *this;
}

NetworkAddress NetworkAddress::fromString(const std::string& address) {
    NetworkAddress addr;
    try {
        addr._impl->address = asio::ip::make_address(address);
    } catch (const std::exception&) {
    }
    return addr;
}

NetworkAddress NetworkAddress::anyV4() {
    NetworkAddress addr;
    addr._impl->address = asio::ip::address_v4::any();
    return addr;
}

NetworkAddress NetworkAddress::anyV6() {
    NetworkAddress addr;
    addr._impl->address = asio::ip::address_v6::any();
    return addr;
}

NetworkAddress NetworkAddress::loopbackV4() {
    NetworkAddress addr;
    addr._impl->address = asio::ip::address_v4::loopback();
    return addr;
}

NetworkAddress NetworkAddress::loopbackV6() {
    NetworkAddress addr;
    addr._impl->address = asio::ip::address_v6::loopback();
    return addr;
}

bool NetworkAddress::isV4() const {
    return _impl->address.is_v4();
}

bool NetworkAddress::isV6() const {
    return _impl->address.is_v6();
}

std::string NetworkAddress::toString() const {
    return _impl->address.to_string();
}

void* NetworkAddress::getInternalAddress() {
    return &(_impl->address);
}

const void* NetworkAddress::getInternalAddress() const {
    return &(_impl->address);
}

void NetworkAddress::setFromInternal(void* internalAddr) {
    if (internalAddr) {
        _impl->address = *static_cast<asio::ip::address*>(internalAddr);
    }
}

} // namespace net
