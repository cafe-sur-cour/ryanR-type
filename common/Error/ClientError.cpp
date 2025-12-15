/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ClientError
*/

#include "ClientError.hpp"
#include <string>

namespace err {

ClientError::ClientError(const std::string &message, ErrorCode code)
    : AError(message, static_cast<int>(code)) {
}

ClientError::~ClientError() {
}

std::string ClientError::getType() const noexcept {
    return "ClientError";
}

}  // namespace err
