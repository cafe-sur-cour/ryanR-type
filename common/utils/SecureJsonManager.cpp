/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Secure JSON File Manager
*/

#include <fstream>
#include <string>

#include "SecureJsonManager.hpp"
#include "Encryption.hpp"

namespace utils {

nlohmann::json SecureJsonManager::readSecureJson(const std::string& filepath) {
    if (!std::filesystem::exists(filepath)) {
        return nlohmann::json::array();
    }

    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        return nlohmann::json::array();
    }

    std::string encryptedContent((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());
    file.close();
    if (encryptedContent.empty()) {
        return nlohmann::json::array();
    }

    try {
        std::string decrypted = Encryption::decrypt(encryptedContent);
        return nlohmann::json::parse(decrypted);
    } catch (const std::exception&) {
        try {
            return nlohmann::json::parse(encryptedContent);
        } catch (const std::exception&) {
            return nlohmann::json::array();
        }
    }
}

bool SecureJsonManager::writeSecureJson(const std::string& filepath,
    const nlohmann::json& data) {
    try {
        std::filesystem::path path(filepath);
        if (path.has_parent_path()) {
            std::filesystem::create_directories(path.parent_path());
        }

        std::string jsonString = data.dump(4);
        std::string encrypted = Encryption::encrypt(jsonString);
        std::ofstream file(filepath, std::ios::binary);
        if (!file.is_open()) {
            return false;
        }

        file << encrypted;
        file.close();
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

}  // namespace utils
