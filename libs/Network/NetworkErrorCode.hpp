/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** NetworkErrorCode - Abstraction pour les codes d'erreur réseau
*/

#ifndef NETWORK_ERROR_CODE_HPP
#define NETWORK_ERROR_CODE_HPP

#include <string>

namespace net {

enum class NetworkError {
    SUCCESS = 0,
    WOULD_BLOCK,
    AGAIN,
    CONNECTION_REFUSED,
    NETWORK_UNREACHABLE,
    TIMED_OUT,
    OTHER
};

class NetworkErrorCode {
    public:
        NetworkErrorCode();
        ~NetworkErrorCode() = default;

        void clear();
        bool hasError() const;
        explicit operator bool() const { return hasError(); }
        std::string message() const;
        NetworkError getError() const;
        void setError(NetworkError error, const std::string& msg = "");
        bool operator==(NetworkError error) const;
        bool operator!=(NetworkError error) const;

        void* getInternalErrorCode();
        const void* getInternalErrorCode() const;
        void setFromInternal(void* internalEc);

    private:
        class Impl;
        Impl* _impl;
};

} // namespace net

#endif // NETWORK_ERROR_CODE_HPP
