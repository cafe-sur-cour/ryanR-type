/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** NetworkAddress implementation
*/

#include <asio.hpp>
#include "AsioAddress.hpp"

namespace net {

class AsioAddress::Impl {
public:
    asio::ip::address address;
};

}

net::AsioAddress::AsioAddress() : _impl(std::make_unique<Impl>()) {
}

net::AsioAddress::~AsioAddress() = default;

net::AsioAddress::AsioAddress(const AsioAddress& other) : _impl(std::make_unique<Impl>()) {
    _impl->address = other._impl->address;
}

net::AsioAddress& net::AsioAddress::operator=(const AsioAddress& other) {
    if (this != &other) {
        _impl->address = other._impl->address;
    }
    return *this;
}

net::AsioAddress net::AsioAddress::fromString(const std::string& address) {
    net::AsioAddress addr;
    try {
        addr._impl->address = asio::ip::make_address(address);
    } catch (const std::exception&) {
    }
    return addr;
}

net::AsioAddress net::AsioAddress::anyV4() {
    net::AsioAddress addr;
    addr._impl->address = asio::ip::address_v4::any();
    return addr;
}

net::AsioAddress net::AsioAddress::anyV6() {
    net::AsioAddress addr;
    addr._impl->address = asio::ip::address_v6::any();
    return addr;
}

net::AsioAddress net::AsioAddress::loopbackV4() {
    net::AsioAddress addr;
    addr._impl->address = asio::ip::address_v4::loopback();
    return addr;
}

net::AsioAddress net::AsioAddress::loopbackV6() {
    net::AsioAddress addr;
    addr._impl->address = asio::ip::address_v6::loopback();
    return addr;
}

bool net::AsioAddress::isV4() const {
    return _impl->address.is_v4();
}

bool net::AsioAddress::isV6() const {
    return _impl->address.is_v6();
}

std::string net::AsioAddress::toString() const {
    return _impl->address.to_string();
}

std::shared_ptr<void> net::AsioAddress::getInternalAddress() {
    return std::shared_ptr<void>(&(_impl->address), [](void*){});
}

std::shared_ptr<const void> net::AsioAddress::getInternalAddress() const {
    return std::shared_ptr<const void>(&(_impl->address), [](const void*){});
}

void net::AsioAddress::setFromInternal(std::shared_ptr<void> internalAddr) {
    if (internalAddr) {
        _impl->address = *static_cast<asio::ip::address*>(internalAddr.get());
    }
}

net::INetworkAddress& net::AsioAddress::operator=(const INetworkAddress& other) {
    if (this != &other) {
        auto otherInternal = const_cast<INetworkAddress&>(other).getInternalAddress();
        if (otherInternal) {
            _impl->address = *static_cast<asio::ip::address*>(otherInternal.get());
        }
    }
    return *this;
}
