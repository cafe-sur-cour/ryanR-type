/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** NetworkErrorCode - Abstraction pour les codes d'erreur réseau
*/

#ifndef NETWORK_ERROR_CODE_HPP
#define NETWORK_ERROR_CODE_HPP

#include <string>
#include <memory>
#include "../INetworkErrorCode.hpp"

namespace net {

class AsioErrorCode : public INetworkErrorCode {
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
        class Impl;
        std::unique_ptr<Impl> _impl;
};

} // namespace net

#endif // NETWORK_ERROR_CODE_HPP
