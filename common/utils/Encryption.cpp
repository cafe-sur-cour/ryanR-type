/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Encryption utilities
*/

#include "Encryption.hpp"
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

namespace utils {

std::string Encryption::getEncryptionKey() {
    const char* envKey = std::getenv("ENCRYPTION_KEY");
    if (envKey && std::string(envKey).length() > 0) {
        return std::string(envKey);
    }

    std::ifstream envFile("common/utils/.env");
    if (envFile.is_open()) {
        std::string line;
        while (std::getline(envFile, line)) {
            if (line.find("ENCRYPTION_KEY=") == 0) {
                std::string key = line.substr(15);
                if (key.length() > 1 && key[0] == '"' && key.back() == '"') {
                    key = key.substr(1, key.length() - 2);
                }
                if (!key.empty()) {
                    return key;
                }
            }
        }
        envFile.close();
    }
    return nullptr;
}

std::string Encryption::encrypt(const std::string& data) {
    std::vector<unsigned char> encrypted;
    std::string key = getEncryptionKey();
    size_t keyLen = key.length();

    for (size_t i = 0; i < data.length(); ++i) {
        encrypted.push_back(static_cast<unsigned char>(data[i] ^ key[i % keyLen]));
    }
    return base64Encode(encrypted);
}

std::string Encryption::decrypt(const std::string& encryptedData) {
    std::vector<unsigned char> decoded = base64Decode(encryptedData);
    std::string decrypted;
    std::string key = getEncryptionKey();
    size_t keyLen = key.length();

    for (size_t i = 0; i < decoded.size(); ++i) {
        decrypted += static_cast<char>(decoded[i] ^ key[i % keyLen]);
    }
    return decrypted;
}

std::string Encryption::base64Encode(const std::vector<unsigned char>& data) {
    static const char* base64Chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    std::string encoded;
    int val = 0;
    int valb = -6;
    for (unsigned char c : data) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            encoded.push_back(base64Chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }

    if (valb > -6) {
        encoded.push_back(base64Chars[((val << 8) >> (valb + 8)) & 0x3F]);
    }

    while (encoded.size() % 4) {
        encoded.push_back('=');
    }
    return encoded;
}

std::vector<unsigned char> Encryption::base64Decode(const std::string& encoded) {
    static const int base64Decode[256] = {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
        -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
        -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
        41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
    };

    std::vector<unsigned char> decoded;
    int val = 0;
    int valb = -8;
    for (char c : encoded) {
        if (base64Decode[static_cast<unsigned char>(c)] == -1) break;
        val = (val << 6) + base64Decode[static_cast<unsigned char>(c)];
        valb += 6;
        if (valb >= 0) {
            decoded.push_back((val >> valb) & 0xFF);
            valb -= 8;
        }
    }
    return decoded;
}

}  // namespace utils
