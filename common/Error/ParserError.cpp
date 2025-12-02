/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ParserError
*/

#include "ParserError.hpp"
#include <string>

namespace err {

ParserError::ParserError(const std::string &message, ErrorCode code)
    : AError(message, static_cast<int>(code)) {}

std::string ParserError::getType() const noexcept {
    return "ParserError";
}
}  // namespace err
