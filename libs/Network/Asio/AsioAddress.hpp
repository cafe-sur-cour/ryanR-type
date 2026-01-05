/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** NetworkAddress
*/

#ifndef NETWORK_ADDRESS_HPP
#define NETWORK_ADDRESS_HPP

#include <string>
#include <memory>
#include <asio.hpp>
#include "../../../common/interfaces/INetworkAddress.hpp"

namespace net {

class AsioAddress : public INetworkAddress {

    class Impl {
        public:
            asio::ip::address address;
    };

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

        std::shared_ptr<void> getInternalAddress() override;
        std::shared_ptr<const void> getInternalAddress() const override;
        void setFromInternal(std::shared_ptr<void> internalAddr) override;

    private:
        std::unique_ptr<Impl> _impl;
};

} // namespace net

#endif // NETWORK_ADDRESS_HPP
