/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** JsonValidation Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/Parser/Utils/JsonValidation.hpp"

TEST(JsonValidationTest, HasRequiredFieldsAllPresent) {
    nlohmann::json json = {{"name", "test"}, {"value", 42}, {"enabled", true}};

    auto result = parser::JsonValidation::hasRequiredFields(json, {"name", "value", "enabled"});
    EXPECT_TRUE(result.valid);
    EXPECT_TRUE(result.errors.empty());
}

TEST(JsonValidationTest, HasRequiredFieldsSomeMissing) {
    nlohmann::json json = {{"name", "test"}, {"value", 42}};

    auto result = parser::JsonValidation::hasRequiredFields(json, {"name", "value", "missing", "also_missing"});
    EXPECT_FALSE(result.valid);
    EXPECT_EQ(result.errors.size(), 2);
}

TEST(JsonValidationTest, HasRequiredFieldsWithContext) {
    nlohmann::json json = {{"name", "test"}};

    auto result = parser::JsonValidation::hasRequiredFields(json, {"name", "missing"}, "Wave 1");
    EXPECT_FALSE(result.valid);
    EXPECT_EQ(result.errors.size(), 1);
    EXPECT_TRUE(result.errors[0].find("Wave 1") != std::string::npos);
    EXPECT_TRUE(result.errors[0].find("missing") != std::string::npos);
}

TEST(JsonValidationTest, HasFieldOfTypeString) {
    nlohmann::json json = {{"name", "test"}};
    EXPECT_TRUE(parser::JsonValidation::hasFieldOfType(json, "name", "string"));
    EXPECT_FALSE(parser::JsonValidation::hasFieldOfType(json, "name", "number"));
}

TEST(JsonValidationTest, HasFieldOfTypeNumber) {
    nlohmann::json json = {{"value", 42}};
    EXPECT_TRUE(parser::JsonValidation::hasFieldOfType(json, "value", "number"));
    EXPECT_FALSE(parser::JsonValidation::hasFieldOfType(json, "value", "string"));
}

TEST(JsonValidationTest, HasFieldOfTypeArray) {
    nlohmann::json json = {{"items", {1, 2, 3}}};
    EXPECT_TRUE(parser::JsonValidation::hasFieldOfType(json, "items", "array"));
    EXPECT_FALSE(parser::JsonValidation::hasFieldOfType(json, "items", "object"));
}

TEST(JsonValidationTest, HasFieldOfTypeObject) {
    nlohmann::json json = {{"config", {{"key", "value"}}}};
    EXPECT_TRUE(parser::JsonValidation::hasFieldOfType(json, "config", "object"));
    EXPECT_FALSE(parser::JsonValidation::hasFieldOfType(json, "config", "array"));
}

TEST(JsonValidationTest, HasFieldOfTypeBoolean) {
    nlohmann::json json = {{"enabled", true}};
    EXPECT_TRUE(parser::JsonValidation::hasFieldOfType(json, "enabled", "boolean"));
    EXPECT_FALSE(parser::JsonValidation::hasFieldOfType(json, "enabled", "number"));
}

TEST(JsonValidationTest, HasFieldOfTypeMissingField) {
    nlohmann::json json = {{"name", "test"}};
    EXPECT_FALSE(parser::JsonValidation::hasFieldOfType(json, "missing", "string"));
}

TEST(JsonValidationTest, GetOrDefaultReturnsValue) {
    nlohmann::json json = {{"speed", 10.5f}, {"name", "player"}};

    EXPECT_FLOAT_EQ(parser::JsonValidation::getOrDefault(json, "speed", 1.0f), 10.5f);
    EXPECT_EQ(parser::JsonValidation::getOrDefault(json, "name", std::string("default")), "player");
}

TEST(JsonValidationTest, GetOrDefaultReturnsDefault) {
    nlohmann::json json = {{"name", "test"}};

    EXPECT_FLOAT_EQ(parser::JsonValidation::getOrDefault(json, "speed", 1.0f), 1.0f);
    EXPECT_EQ(parser::JsonValidation::getOrDefault(json, "missing", std::string("default")), "default");
}

TEST(JsonValidationTest, GetOrDefaultHandlesTypeError) {
    nlohmann::json json = {{"value", "not_a_number"}};

    // Should return default if type conversion fails
    EXPECT_EQ(parser::JsonValidation::getOrDefault(json, "value", 42), 42);
}

TEST(JsonValidationTest, GetNestedFieldSimple) {
    nlohmann::json json = {{"config", {{"display", {{"width", 1920}}}}}};

    auto result = parser::JsonValidation::getNestedField(json, "config.display.width");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->get<int>(), 1920);
}

TEST(JsonValidationTest, GetNestedFieldNotFound) {
    nlohmann::json json = {{"config", {{"display", {{"width", 1920}}}}}};

    auto result = parser::JsonValidation::getNestedField(json, "config.display.height");
    EXPECT_FALSE(result.has_value());
}

TEST(JsonValidationTest, GetNestedFieldInvalidPath) {
    nlohmann::json json = {{"config", {{"display", {{"width", 1920}}}}}};

    auto result = parser::JsonValidation::getNestedField(json, "config.missing.width");
    EXPECT_FALSE(result.has_value());
}

TEST(JsonValidationTest, GetNestedFieldRoot) {
    nlohmann::json json = {{"name", "test"}};

    auto result = parser::JsonValidation::getNestedField(json, "name");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->get<std::string>(), "test");
}

TEST(JsonValidationTest, ValidationResultBoolOperator) {
    parser::ValidationResult validResult;
    validResult.valid = true;
    EXPECT_TRUE(validResult);

    parser::ValidationResult invalidResult;
    invalidResult.valid = false;
    EXPECT_FALSE(invalidResult);
}
