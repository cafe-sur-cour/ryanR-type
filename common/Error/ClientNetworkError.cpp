/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ClientNetworkError
*/

#include <string>

#include "ClientNetworkError.hpp"

namespace err {


ClientNetworkError::ClientNetworkError(const std::string &message, ErrorCode code)
    : AError(message, static_cast<int>(code)) {}

std::string ClientNetworkError::getType() const noexcept {
    return "ClientNetworkError";
}

}
