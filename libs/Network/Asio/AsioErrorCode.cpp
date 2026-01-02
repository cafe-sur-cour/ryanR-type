/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** NetworkErrorCode implementation
*/

#include <memory>
#include <string>
#include <utility>
#include <asio.hpp>

#include "AsioErrorCode.hpp"

namespace net {

class AsioErrorCode::Impl {
 public:
        asio::error_code asioErrorCode;
};

}

net::AsioErrorCode::AsioErrorCode() : _impl(std::make_unique<Impl>()) {
}

net::AsioErrorCode::~AsioErrorCode() = default;

net::AsioErrorCode& net::AsioErrorCode::operator=(AsioErrorCode&& other) noexcept {
    if (this != &other) {
        _impl = std::move(other._impl);
    }
    return *this;
}

void net::AsioErrorCode::clear() {
    _impl->asioErrorCode.clear();
}

bool net::AsioErrorCode::hasError() const {
    return static_cast<bool>(_impl->asioErrorCode);
}

net::AsioErrorCode::operator bool() const {
    return hasError();
}

std::string net::AsioErrorCode::message() const {
    return _impl->asioErrorCode.message();
}

net::NetworkError net::AsioErrorCode::getError() const {
    if (!hasError()) {
        return NetworkError::SUCCESS;
    }

    const auto& ec = _impl->asioErrorCode;

    if (ec == asio::error::would_block) {
        return NetworkError::WOULD_BLOCK;
    }
    if (ec == asio::error::try_again) {
        return NetworkError::AGAIN;
    }
    if (ec == asio::error::connection_refused) {
        return NetworkError::CONNECTION_REFUSED;
    }
    if (ec == asio::error::network_unreachable) {
        return NetworkError::NETWORK_UNREACHABLE;
    }
    if (ec == asio::error::timed_out) {
        return NetworkError::TIMED_OUT;
    }

    return NetworkError::OTHER;
}

void net::AsioErrorCode::setError(NetworkError error, const std::string& msg) {
    (void)msg;
    switch (error) {
        case NetworkError::SUCCESS:
            _impl->asioErrorCode.clear();
            break;
        case NetworkError::WOULD_BLOCK:
            _impl->asioErrorCode = asio::error::would_block;
            break;
        case NetworkError::AGAIN:
            _impl->asioErrorCode = asio::error::try_again;
            break;
        case NetworkError::CONNECTION_REFUSED:
            _impl->asioErrorCode = asio::error::connection_refused;
            break;
        case NetworkError::NETWORK_UNREACHABLE:
            _impl->asioErrorCode = asio::error::network_unreachable;
            break;
        case NetworkError::TIMED_OUT:
            _impl->asioErrorCode = asio::error::timed_out;
            break;
        default:
            _impl->asioErrorCode = asio::error::make_error_code(
                asio::error::basic_errors::fault);
            break;
    }
}

bool net::AsioErrorCode::operator==(NetworkError error) const {
    return getError() == error;
}

bool net::AsioErrorCode::operator!=(NetworkError error) const {
    return getError() != error;
}

std::shared_ptr<void> net::AsioErrorCode::getInternalErrorCode() {
    return std::shared_ptr<void>(&(_impl->asioErrorCode), [](void*){});
}

std::shared_ptr<const void> net::AsioErrorCode::getInternalErrorCode() const {
    return std::shared_ptr<const void>(&(_impl->asioErrorCode), [](const void*){});
}

void net::AsioErrorCode::setFromInternal(std::shared_ptr<void> internalEc) {
    if (internalEc) {
        _impl->asioErrorCode = *static_cast<asio::error_code*>(internalEc.get());
    }
}
