/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Encryption utilities
*/

#ifndef ENCRYPTION_HPP_
#define ENCRYPTION_HPP_

#include <string>
#include <vector>

namespace utils {

class Encryption {
public:
    static std::string encrypt(const std::string& data);
    static std::string decrypt(const std::string& encryptedData);
    static std::string base64Encode(const std::vector<unsigned char>& data);
    static std::vector<unsigned char> base64Decode(const std::string& encoded);

private:
    static inline const std::string _key = "R-Type_Secure_Key_2026";
};

}  // namespace utils

#endif /* !ENCRYPTION_HPP_ */
