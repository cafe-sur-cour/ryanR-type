/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** NetworkErrorCode
*/

#ifndef INETWORK_ERROR_CODE_HPP
#define INETWORK_ERROR_CODE_HPP

#include <string>
#include <memory>

namespace net {

enum class NetworkError {
    SUCCESS = 0,
    WOULD_BLOCK = 1,
    AGAIN = 2,
    CONNECTION_REFUSED = 3,
    NETWORK_UNREACHABLE = 4,
    TIMED_OUT = 5,
    OTHER = 6
};

class INetworkErrorCode {
    public:
        virtual ~INetworkErrorCode() noexcept = default;

        virtual void clear() = 0;
        virtual bool hasError() const = 0;
        virtual explicit operator bool() const = 0;
        virtual std::string message() const = 0;
        virtual NetworkError getError() const = 0;
        virtual void setError(NetworkError error, const std::string& msg = "") = 0;
        virtual bool operator==(NetworkError error) const = 0;
        virtual bool operator!=(NetworkError error) const = 0;

        virtual std::shared_ptr<void> getInternalErrorCode() = 0;
        virtual std::shared_ptr<const void> getInternalErrorCode() const = 0;
        virtual void setFromInternal(std::shared_ptr<void> internalEc) = 0;
};

} // namespace net

#endif // INETWORK_ERROR_CODE_HPP
