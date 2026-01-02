/*
** EPITECH PROJECT, 2026
** ryanR-type
** File description:
** ScriptingError
*/

#include "ScriptingError.hpp"

namespace err {

ScriptingError::ScriptingError(const std::string &message, ErrorCode code)
    : AError(message, static_cast<int>(code)) {}

std::string ScriptingError::getType() const noexcept {
    return "ScriptingError";
}

}  // namespace err
