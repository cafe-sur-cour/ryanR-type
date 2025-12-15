/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LibrairiesLoadError
*/

#include "LibrairiesLoadError.hpp"
#include <string>

namespace err {

LibrairiesLoadError::LibrairiesLoadError(const std::string &message,
    ErrorCode code) : AError(message, static_cast<int>(code)) {
}

LibrairiesLoadError::~LibrairiesLoadError() {
}


std::string LibrairiesLoadError::getType() const noexcept {
    return "LibrairiesLoadError";
}

}  // namespace err

