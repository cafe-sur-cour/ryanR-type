/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** NetworkAddress - Abstraction pour les adresses IP
*/

#ifndef NETWORK_ADDRESS_HPP
#define NETWORK_ADDRESS_HPP

#include <string>
#include <memory>
#include "../INetworkAddress.hpp"

namespace asio {
    namespace ip {
        class address;
    }
}

namespace net {

class AsioAddress : public INetworkAddress {
    public:
        static AsioAddress fromString(const std::string& address);
        static AsioAddress anyV4();
        static AsioAddress anyV6();
        static AsioAddress loopbackV4();
        static AsioAddress loopbackV6();
        bool isV4() const override;
        bool isV6() const override;
        std::string toString() const override;

        AsioAddress();
        AsioAddress(const AsioAddress& other);
        ~AsioAddress() override;
        AsioAddress& operator=(const AsioAddress& other);
        INetworkAddress& operator=(const INetworkAddress& other) override;

        void* getInternalAddress() override;
        const void* getInternalAddress() const override;
        void setFromInternal(void* internalAddr) override;

    private:
        class Impl;
        std::unique_ptr<Impl> _impl;
};

} // namespace net

#endif // NETWORK_ADDRESS_HPP
