/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Secure JSON File Manager
*/

#ifndef SECURE_JSON_MANAGER_HPP_
#define SECURE_JSON_MANAGER_HPP_

#include <string>
#include <nlohmann/json.hpp>

namespace utils {

class SecureJsonManager {
public:
    static nlohmann::json readSecureJson(const std::string& filepath);
    static bool writeSecureJson(const std::string& filepath, const nlohmann::json& data);
};

}  // namespace utils

#endif /* !SECURE_JSON_MANAGER_HPP_ */
