/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** JsonLoader Tests
*/

#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "../../../../common/Parser/Utils/JsonLoader.hpp"
#include "../../../../common/Error/ParserError.hpp"

class JsonLoaderTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create test files in a temporary directory
        testDir = std::filesystem::temp_directory_path() / "jsonloader_test";
        std::filesystem::create_directories(testDir);

        // Create a valid JSON file
        validJsonPath = testDir / "valid.json";
        std::ofstream validFile(validJsonPath);
        validFile << R"({"name": "test", "value": 42})";
        validFile.close();

        // Create an invalid JSON file
        invalidJsonPath = testDir / "invalid.json";
        std::ofstream invalidFile(invalidJsonPath);
        invalidFile << R"({"name": "test", "value": )";  // Invalid JSON
        invalidFile.close();

        nonexistentPath = testDir / "nonexistent.json";
    }

    void TearDown() override {
        // Clean up test files
        std::filesystem::remove_all(testDir);
    }

    std::filesystem::path testDir;
    std::filesystem::path validJsonPath;
    std::filesystem::path invalidJsonPath;
    std::filesystem::path nonexistentPath;
};

TEST_F(JsonLoaderTest, LoadValidFile) {
    auto json = parser::JsonLoader::loadFromFile(validJsonPath.string());
    EXPECT_TRUE(json.contains("name"));
    EXPECT_EQ(json["name"].get<std::string>(), "test");
    EXPECT_EQ(json["value"].get<int>(), 42);
}

TEST_F(JsonLoaderTest, ThrowsOnMissingFile) {
    EXPECT_THROW(
        parser::JsonLoader::loadFromFile(nonexistentPath.string()),
        err::ParserError
    );
}

TEST_F(JsonLoaderTest, ThrowsOnInvalidJson) {
    EXPECT_THROW(
        parser::JsonLoader::loadFromFile(invalidJsonPath.string()),
        err::ParserError
    );
}

TEST_F(JsonLoaderTest, TryLoadReturnsSuccessOnValidFile) {
    auto result = parser::JsonLoader::tryLoadFromFile(validJsonPath.string());
    EXPECT_TRUE(result.success);
    EXPECT_TRUE(result.data.contains("name"));
    EXPECT_TRUE(result.errorMessage.empty());
}

TEST_F(JsonLoaderTest, TryLoadReturnsErrorOnFailure) {
    auto result = parser::JsonLoader::tryLoadFromFile(nonexistentPath.string());
    EXPECT_FALSE(result.success);
    EXPECT_FALSE(result.errorMessage.empty());
}

TEST_F(JsonLoaderTest, FileExistsReturnsTrueForValidFile) {
    EXPECT_TRUE(parser::JsonLoader::fileExists(validJsonPath.string()));
}

TEST_F(JsonLoaderTest, FileExistsReturnsFalseForNonexistent) {
    EXPECT_FALSE(parser::JsonLoader::fileExists(nonexistentPath.string()));
}

TEST_F(JsonLoaderTest, GetExtensionReturnsCorrectExtension) {
    EXPECT_EQ(parser::JsonLoader::getExtension("test.json"), ".json");
    EXPECT_EQ(parser::JsonLoader::getExtension("path/to/file.txt"), ".txt");
    EXPECT_EQ(parser::JsonLoader::getExtension("noextension"), "");
}

TEST_F(JsonLoaderTest, ParseFromStringValid) {
    std::string jsonString = R"({"key": "value", "number": 123})";
    auto json = parser::JsonLoader::parseFromString(jsonString, "test");
    EXPECT_TRUE(json.contains("key"));
    EXPECT_EQ(json["key"].get<std::string>(), "value");
    EXPECT_EQ(json["number"].get<int>(), 123);
}

TEST_F(JsonLoaderTest, ParseFromStringThrowsOnInvalid) {
    std::string invalidJsonString = R"({"key": "value", "number": })";
    EXPECT_THROW(
        parser::JsonLoader::parseFromString(invalidJsonString, "test"),
        err::ParserError
    );
}

TEST_F(JsonLoaderTest, TryParseFromStringReturnsSuccess) {
    std::string jsonString = R"({"valid": true})";
    auto result = parser::JsonLoader::tryParseFromString(jsonString);
    EXPECT_TRUE(result.success);
    EXPECT_TRUE(result.data.contains("valid"));
}

TEST_F(JsonLoaderTest, TryParseFromStringReturnsError) {
    std::string invalidJsonString = R"({"invalid": })";
    auto result = parser::JsonLoader::tryParseFromString(invalidJsonString);
    EXPECT_FALSE(result.success);
    EXPECT_FALSE(result.errorMessage.empty());
}
