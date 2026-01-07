/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** JsonLoader implementation
*/

#include "JsonLoader.hpp"
#include <fstream>
#include <sstream>
#include <filesystem>  // NOLINT(build/c++17)
#include <string>
#include "../../Error/ParserError.hpp"

namespace parser {

nlohmann::json JsonLoader::loadFromFile(const std::string& filePath) {
    if (!fileExists(filePath)) {
        throw err::ParserError(
            "Cannot open file: " + filePath,
            err::ParserError::FILE_NOT_FOUND
        );
    }

    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw err::ParserError(
            "Cannot open file: " + filePath,
            err::ParserError::FILE_NOT_FOUND
        );
    }

    nlohmann::json jsonData;
    try {
        file >> jsonData;
    } catch (const nlohmann::detail::exception& e) {
        throw err::ParserError(
            "Invalid JSON format in file: " + filePath + " (" + e.what() + ")",
            err::ParserError::INVALID_FORMAT
        );
    }

    return jsonData;
}

JsonLoader::LoadResult JsonLoader::tryLoadFromFile(const std::string& filePath) noexcept {
    LoadResult result;
    result.success = false;

    try {
        result.data = loadFromFile(filePath);
        result.success = true;
    } catch (const err::ParserError& e) {
        result.errorMessage = e.what();
    } catch (const std::exception& e) {
        result.errorMessage = std::string("Unexpected error: ") + e.what();
    }

    return result;
}

nlohmann::json JsonLoader::parseFromString(
    const std::string& jsonString,
    const std::string& sourceName
) {
    try {
        return nlohmann::json::parse(jsonString);
    } catch (const nlohmann::detail::exception& e) {
        throw err::ParserError(
            "Invalid JSON format in " + sourceName + ": " + e.what(),
            err::ParserError::INVALID_FORMAT
        );
    }
}

JsonLoader::LoadResult JsonLoader::tryParseFromString(const std::string& jsonString) noexcept {
    LoadResult result;
    result.success = false;

    try {
        result.data = nlohmann::json::parse(jsonString);
        result.success = true;
    } catch (const nlohmann::detail::exception& e) {
        result.errorMessage = e.what();
    }

    return result;
}

bool JsonLoader::fileExists(const std::string& filePath) {
    return std::filesystem::exists(filePath) &&
           std::filesystem::is_regular_file(filePath);
}

std::string JsonLoader::getExtension(const std::string& filePath) {
    return std::filesystem::path(filePath).extension().string();
}

}  // namespace parser
