/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** NetworkErrorCode
*/

#ifndef NETWORK_ERROR_CODE_HPP
#define NETWORK_ERROR_CODE_HPP

#include <string>
#include <memory>
#include <asio.hpp>
#include "../INetworkErrorCode.hpp"

namespace net {

class AsioErrorCode : public INetworkErrorCode {

    class Impl {
        public:
            asio::error_code asioErrorCode;
    };

    public:
        AsioErrorCode();
        ~AsioErrorCode() override;

        void clear() override;
        bool hasError() const override;
        explicit operator bool() const override;
        std::string message() const override;
        NetworkError getError() const override;
        void setError(NetworkError error, const std::string& msg = "") override;
        bool operator==(NetworkError error) const override;
        bool operator!=(NetworkError error) const override;

        std::shared_ptr<void> getInternalErrorCode() override;
        std::shared_ptr<const void> getInternalErrorCode() const override;
        void setFromInternal(std::shared_ptr<void> internalEc) override;

        AsioErrorCode(const AsioErrorCode& other);
        AsioErrorCode& operator=(const AsioErrorCode& other);

        AsioErrorCode(AsioErrorCode&& other) noexcept;
        AsioErrorCode& operator=(AsioErrorCode&& other) noexcept;

    private:
        std::unique_ptr<Impl> _impl;
};

} // namespace net

#endif // NETWORK_ERROR_CODE_HPP
