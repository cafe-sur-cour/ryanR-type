/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ServerErrror
*/
#include <string>

#include "ServerErrror.hpp"

namespace err {


ServerError::ServerError(const std::string &message, ErrorCode code)
    : AError(message, static_cast<int>(code)) {}

std::string ServerError::getType() const noexcept {
    return "ServerError";
}

}
