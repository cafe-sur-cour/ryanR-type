/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AError
*/
#include <string>

#include "AError.hpp"

namespace err {

AError::AError(const std::string &message, int code)
    : m_message(message), m_code(code) {}

const char* AError::what() const noexcept {
    return m_message.c_str();
}

int AError::getCode() const noexcept {
    return m_code;
}

std::string AError::getDetails() const noexcept {
    return "[" + getType() + "] Code: " + std::to_string(m_code) +
           " - " + m_message;
}

}
