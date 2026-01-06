/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** JsonLoader
*/

#ifndef JSONLOADER_HPP_
#define JSONLOADER_HPP_

#include <string>
#include <optional>
#include <nlohmann/json.hpp>

namespace parser {

class JsonLoader {
    public:
        struct LoadResult {
            nlohmann::json data;
            bool success;
            std::string errorMessage;
        };

        static nlohmann::json loadFromFile(const std::string& filePath);

        static LoadResult tryLoadFromFile(const std::string& filePath) noexcept;

        static nlohmann::json parseFromString(
            const std::string& jsonString,
            const std::string& sourceName = "string"
        );

        static LoadResult tryParseFromString(const std::string& jsonString) noexcept;

        static bool fileExists(const std::string& filePath);

        static std::string getExtension(const std::string& filePath);

    private:
        JsonLoader() = delete;
};

}  // namespace parser

#endif /* !JSONLOADER_HPP_ */
