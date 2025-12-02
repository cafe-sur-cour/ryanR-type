/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Parser tests
*/

#include <gtest/gtest.h>
#include <memory>
#include <filesystem>
#include <fstream>
#include <string>
#include "../../common/Parser/Parser.hpp"
#include "../../common/Prefab/ParsedEntityPrefab.hpp"
#include "../../common/Error/ParserError.hpp"
#include "../../common/Prefab/entityPrefabManager/EntityPrefabManager.hpp"
#include "../../common/ECS/entity/registry/Registry.hpp"

namespace {

class ParserTest : public ::testing::Test {
protected:
    void SetUp() override {
        testDir = std::filesystem::temp_directory_path() / "parser_tests";
        std::filesystem::create_directories(testDir);

        registry = std::make_shared<ecs::Registry>();

        prefabManager = std::make_shared<EntityPrefabManager>();

        parser = std::make_shared<Parser>(prefabManager, CLIENT);
    }

    void TearDown() override {
        std::filesystem::remove_all(testDir);
    }

    void createTestFile(const std::string& filename, const std::string& content) {
        std::ofstream file(testDir / filename);
        file << content;
        file.close();
    }

    std::filesystem::path testDir;
    std::shared_ptr<ecs::Registry> registry;
    std::shared_ptr<EntityPrefabManager> prefabManager;
    std::shared_ptr<Parser> parser;
};

// Test parsing a valid entity
TEST_F(ParserTest, ParseValidEntity) {
    std::string jsonContent = R"(
    {
        "name": "TestEntity",
        "components": {
            "TransformComponent": {
                "target": "client",
                "position": {"x": 10.0, "y": 20.0},
                "scale": {"x": 1.0, "y": 1.0},
                "rotation": 0.0
            }
        }
    })";

    createTestFile("valid_entity.json", jsonContent);

    EXPECT_NO_THROW({
        parser->parseEntity((testDir / "valid_entity.json").string());
    });

    // Check that prefab was registered
    auto prefab = prefabManager->getPrefab("TestEntity");
    ASSERT_NE(prefab, nullptr);

    // Check that we can instantiate it
    EXPECT_NO_THROW({
        ecs::Entity entity = prefab->instantiate(registry);
        EXPECT_NE(entity, 0);
    });
}

// Test parsing entity with missing file
TEST_F(ParserTest, ParseEntityFileNotFound) {
    EXPECT_THROW({
        parser->parseEntity((testDir / "nonexistent.json").string());
    }, err::ParserError);

    try {
        parser->parseEntity((testDir / "nonexistent.json").string());
    } catch (const err::ParserError& e) {
        EXPECT_EQ(e.getCode(), static_cast<int>(err::ParserError::FILE_NOT_FOUND));
        EXPECT_NE(std::string(e.what()).find("Cannot open file"), std::string::npos);
    }
}

// Test parsing entity with invalid JSON
TEST_F(ParserTest, ParseEntityInvalidJson) {
    createTestFile("invalid.json", "{ invalid json content }");

    EXPECT_THROW({
        parser->parseEntity((testDir / "invalid.json").string());
    }, err::ParserError);

    try {
        parser->parseEntity((testDir / "invalid.json").string());
    } catch (const err::ParserError& e) {
        EXPECT_EQ(e.getCode(), static_cast<int>(err::ParserError::INVALID_FORMAT));
        EXPECT_NE(std::string(e.what()).find("Invalid JSON format"), std::string::npos);
    }
}

// Test parsing entity with unknown component
TEST_F(ParserTest, ParseEntityUnknownComponent) {
    std::string jsonContent = R"(
    {
        "name": "TestEntity",
        "components": {
            "UnknownComponent": {
                "target": "client"
            }
        }
    })";

    createTestFile("unknown_component.json", jsonContent);

    EXPECT_THROW({
        parser->parseEntity((testDir / "unknown_component.json").string());
    }, err::ParserError);

    try {
        parser->parseEntity((testDir / "unknown_component.json").string());
    } catch (const err::ParserError& e) {
        EXPECT_EQ(e.getCode(), static_cast<int>(err::ParserError::UNKNOWN));
        EXPECT_NE(std::string(e.what()).find("Unknown component"), std::string::npos);
    }
}

// Test parsing entity with missing required field
TEST_F(ParserTest, ParseEntityMissingField) {
    std::string jsonContent = R"(
    {
        "name": "TestEntity",
        "components": {
            "TransformComponent": {
                "target": "client",
                "position": {"x": 10.0, "y": 20.0},
                "scale": {"x": 1.0, "y": 1.0}
            }
        }
    })";

    createTestFile("missing_field.json", jsonContent);

    EXPECT_THROW({
        parser->parseEntity((testDir / "missing_field.json").string());
    }, err::ParserError);

    try {
        parser->parseEntity((testDir / "missing_field.json").string());
    } catch (const err::ParserError& e) {
        EXPECT_EQ(e.getCode(), static_cast<int>(err::ParserError::MISSING_FIELD));
        EXPECT_NE(std::string(e.what()).find("Missing field"), std::string::npos);
    }
}

// Test parsing entity with invalid field type
TEST_F(ParserTest, ParseEntityInvalidFieldType) {
    std::string jsonContent = R"(
    {
        "name": "TestEntity",
        "components": {
            "TransformComponent": {
                "target": "client",
                "position": "invalid_position_format",
                "scale": {"x": 1.0, "y": 1.0},
                "rotation": 0.0
            }
        }
    })";

    createTestFile("invalid_field_type.json", jsonContent);

    EXPECT_THROW({
        parser->parseEntity((testDir / "invalid_field_type.json").string());
    }, err::ParserError);

    try {
        parser->parseEntity((testDir / "invalid_field_type.json").string());
    } catch (const err::ParserError& e) {
        EXPECT_EQ(e.getCode(), static_cast<int>(err::ParserError::INVALID_FORMAT));
        EXPECT_NE(std::string(e.what()).find("Invalid Vector2f format"), std::string::npos);
    }
}

// Test parsing entity with component not targeted for client
TEST_F(ParserTest, ParseEntityComponentNotForClient) {
    std::string jsonContent = R"(
    {
        "name": "TestEntity",
        "components": {
            "TransformComponent": {
                "target": "server",
                "position": {"x": 10.0, "y": 20.0},
                "scale": {"x": 1.0, "y": 1.0},
                "rotation": 0.0
            }
        }
    })";

    createTestFile("server_only.json", jsonContent);

    EXPECT_NO_THROW({
        parser->parseEntity((testDir / "server_only.json").string());
    });

    // Check that prefab was registered but has no components
    auto prefab = prefabManager->getPrefab("TestEntity");
    ASSERT_NE(prefab, nullptr);

    auto parsedPrefab = std::static_pointer_cast<ParsedEntityPrefab>(prefab);
    EXPECT_TRUE(parsedPrefab->getComponents().empty());
}

// Test prefab instantiation with unknown component type
TEST_F(ParserTest, PrefabInstantiateUnknownComponent) {
    // Create a prefab with a component that has an unknown type_index
    auto prefab = std::make_shared<ParsedEntityPrefab>("TestPrefab", parser->getComponentAdders());

    // Add a mock component with unknown type
    auto mockComponent = std::make_shared<ecs::TransformComponent>(
        math::Vector2f(0.0f, 0.0f), 0.0f, math::Vector2f(1.0f, 1.0f)
    );
    std::type_index unknownType(typeid(void)); // Use void as unknown type
    prefab->addComponent(mockComponent, unknownType);

    EXPECT_THROW({
        prefab->instantiate(registry);
    }, err::ParserError);

    try {
        prefab->instantiate(registry);
    } catch (const err::ParserError& e) {
        EXPECT_EQ(e.getCode(), static_cast<int>(err::ParserError::UNKNOWN));
        EXPECT_NE(std::string(e.what()).find("Unknown component type"), std::string::npos);
    }
}

// Test EntityPrefabManager with non-existent prefab
TEST_F(ParserTest, EntityPrefabManagerGetNonExistentPrefab) {
    EXPECT_THROW({
        prefabManager->createEntityFromPrefab("NonExistent", registry);
    }, err::ParserError);

    try {
        prefabManager->createEntityFromPrefab("NonExistent", registry);
    } catch (const err::ParserError& e) {
        EXPECT_EQ(e.getCode(), static_cast<int>(err::ParserError::UNKNOWN));
        EXPECT_NE(std::string(e.what()).find("not found"), std::string::npos);
    }
}

} // namespace