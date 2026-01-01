/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** NetworkErrorCode implementation
*/

#include "NetworkErrorCode.hpp"
#include <asio.hpp>

namespace net {

class NetworkErrorCode::Impl {
    public:
        asio::error_code asioErrorCode;
};

NetworkErrorCode::NetworkErrorCode() : _impl(new Impl()) {
}

void NetworkErrorCode::clear() {
    _impl->asioErrorCode.clear();
}

bool NetworkErrorCode::hasError() const {
    return static_cast<bool>(_impl->asioErrorCode);
}

NetworkErrorCode::operator bool() const {
    return hasError();
}

std::string NetworkErrorCode::message() const {
    return _impl->asioErrorCode.message();
}

NetworkError NetworkErrorCode::getError() const {
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

void NetworkErrorCode::setError(NetworkError error, const std::string& msg) {
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
            _impl->asioErrorCode = asio::error::make_error_code(asio::error::basic_errors::fault);
            break;
    }
}

bool NetworkErrorCode::operator==(NetworkError error) const {
    return getError() == error;
}

bool NetworkErrorCode::operator!=(NetworkError error) const {
    return getError() != error;
}

void* NetworkErrorCode::getInternalErrorCode() {
    return &(_impl->asioErrorCode);
}

const void* NetworkErrorCode::getInternalErrorCode() const {
    return &(_impl->asioErrorCode);
}

void NetworkErrorCode::setFromInternal(void* internalEc) {
    if (internalEc) {
        _impl->asioErrorCode = *static_cast<asio::error_code*>(internalEc);
    }
}

} // namespace net
