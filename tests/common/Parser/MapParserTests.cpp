/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** MapParser tests
*/

#include <gtest/gtest.h>
#include <memory>
#include <filesystem>
#include <fstream>
#include <string>
#include "../../common/Parser/MapParser/MapParser.hpp"
#include "../../common/Prefab/entityPrefabManager/EntityPrefabManager.hpp"
#include "../../common/ECS/entity/registry/Registry.hpp"
#include "../../common/ECS/entity/factory/IEntityFactory.hpp"
#include "../../common/ECS/entity/factory/EntityFactory.hpp"
#include "../../common/ECS/entity/EntityCreationContext.hpp"
#include "../../common/Error/ParserError.hpp"
#include "../../common/Prefab/IPrefab.hpp"
#include "../../common/constants.hpp"
#include "../../common/components/permanent/TransformComponent.hpp"

// Mock prefab for testing
class MockPrefab : public IPrefab {
public:
    MockPrefab(const std::string& name) : _name(name) {}
    ~MockPrefab() override = default;

    ecs::Entity instantiate(const std::shared_ptr<ecs::Registry>& registry) override {
        ecs::Entity entity = registry->createEntity();
        // Register component type
        registry->registerComponent<ecs::TransformComponent>();
        // Add a basic transform component for testing
        auto transform = std::make_shared<ecs::TransformComponent>();
        registry->addComponent(entity, transform);
        return entity;
    }

    ecs::Entity instantiate(
        const std::shared_ptr<ecs::Registry>& registry,
        const std::shared_ptr<ecs::IEntityFactory>& factory,
        const ecs::EntityCreationContext& context
    ) override {
        ecs::Entity entity = factory->createEntity(registry, context);
        // Register component type
        registry->registerComponent<ecs::TransformComponent>();
        // Add a basic transform component for testing
        auto transform = std::make_shared<ecs::TransformComponent>();
        registry->addComponent(entity, transform);
        return entity;
    }

private:
    std::string _name;
};

namespace {

class MapParserTest : public ::testing::Test {
protected:
    void SetUp() override {
        testDir = std::filesystem::temp_directory_path() / "map_parser_tests";
        std::filesystem::create_directories(testDir);

        registry = std::make_shared<ecs::Registry>();
        prefabManager = std::make_shared<EntityPrefabManager>();

        // Register some mock prefabs for testing
        prefabManager->registerPrefab("background", std::make_shared<MockPrefab>("background"));
        prefabManager->registerPrefab("wall", std::make_shared<MockPrefab>("wall"));
        prefabManager->registerPrefab("enemy1", std::make_shared<MockPrefab>("enemy1"));
        prefabManager->registerPrefab("enemy2", std::make_shared<MockPrefab>("enemy2"));
        prefabManager->registerPrefab("gamezone", std::make_shared<MockPrefab>("gamezone"));
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
};

// Test constructor with null prefabManager
TEST_F(MapParserTest, ConstructorNullPrefabManager) {
    EXPECT_THROW({
        MapParser parser(nullptr, registry);
    }, err::ParserError);
}

// Test constructor with null registry
TEST_F(MapParserTest, ConstructorNullRegistry) {
    EXPECT_THROW({
        MapParser parser(prefabManager, nullptr);
    }, err::ParserError);
}

// Test constructor with valid parameters
TEST_F(MapParserTest, ConstructorValidParameters) {
    EXPECT_NO_THROW({
        MapParser parser(prefabManager, registry);
    });
}

// Test parsing non-existent file
TEST_F(MapParserTest, ParseMapFromFileNotFound) {
    MapParser parser(prefabManager, registry);

    EXPECT_THROW({
        parser.parseMapFromFile((testDir / "nonexistent.json").string());
    }, err::ParserError);

    try {
        parser.parseMapFromFile((testDir / "nonexistent.json").string());
    } catch (const err::ParserError& e) {
        EXPECT_EQ(e.getCode(), static_cast<int>(err::ParserError::FILE_NOT_FOUND));
    }
}

// Test parsing invalid JSON
TEST_F(MapParserTest, ParseMapFromFileInvalidJson) {
    MapParser parser(prefabManager, registry);
    createTestFile("invalid.json", "{ invalid json content }");

    EXPECT_THROW({
        parser.parseMapFromFile((testDir / "invalid.json").string());
    }, err::ParserError);

    try {
        parser.parseMapFromFile((testDir / "invalid.json").string());
    } catch (const err::ParserError& e) {
        EXPECT_EQ(e.getCode(), static_cast<int>(err::ParserError::INVALID_FORMAT));
    }
}

// Test parsing valid map with background
TEST_F(MapParserTest, ParseMapWithBackground) {
    MapParser parser(prefabManager, registry);

    std::string jsonContent = R"(
    {
        "background": "background",
        "scrollSpeed": 2.5
    })";

    createTestFile("map_background.json", jsonContent);

    EXPECT_NO_THROW({
        parser.parseMapFromFile((testDir / "map_background.json").string());
    });

    // Check that background entity was created
    // Note: We can't easily check the exact entity count without more registry introspection
    // but the test passes if no exception is thrown
}

// Test parsing map with custom tile size
TEST_F(MapParserTest, ParseMapWithCustomTileSize) {
    MapParser parser(prefabManager, registry);

    std::string jsonContent = R"(
    {
        "tile": {
            "width": 32.0,
            "height": 32.0
        }
    })";

    createTestFile("map_tilesize.json", jsonContent);

    EXPECT_NO_THROW({
        parser.parseMapFromFile((testDir / "map_tilesize.json").string());
    });
}

// Test parsing map with legend and grid
TEST_F(MapParserTest, ParseMapWithLegendAndGrid) {
    MapParser parser(prefabManager, registry);

    std::string jsonContent = R"(
    {
        "legend": {
            "W": "wall",
            ".": "empty"
        },
        "map": [
            "W W W",
            "W . W",
            "W W W"
        ]
    })";

    createTestFile("map_grid.json", jsonContent);

    EXPECT_NO_THROW({
        parser.parseMapFromFile((testDir / "map_grid.json").string());
    });
}

// Test parsing map with waves
TEST_F(MapParserTest, ParseMapWithWaves) {
    MapParser parser(prefabManager, registry);

    std::string jsonContent = R"(
    {
        "waves": [
            {
                "spawnLength": 10,
                "posX": 5,
                "enemies": [
                    {
                        "type": "enemy1",
                        "count": 3
                    },
                    {
                        "type": "enemy2",
                        "count": 2
                    }
                ]
            }
        ]
    })";

    createTestFile("map_waves.json", jsonContent);

    EXPECT_NO_THROW({
        parser.parseMapFromFile((testDir / "map_waves.json").string());
    });
}

// Test parsing complete map
TEST_F(MapParserTest, ParseCompleteMap) {
    MapParser parser(prefabManager, registry);

    std::string jsonContent = R"(
    {
        "background": "background",
        "scrollSpeed": 1.0,
        "tile": {
            "width": 64.0,
            "height": 64.0
        },
        "legend": {
            "W": "wall",
            ".": "empty"
        },
        "map": [
            "W . W",
            ". W ."
        ],
        "waves": [
            {
                "spawnLength": 5,
                "posX": 10,
                "enemies": [
                    {
                        "type": "enemy1",
                        "count": 1
                    }
                ]
            }
        ]
    })";

    createTestFile("complete_map.json", jsonContent);

    EXPECT_NO_THROW({
        parser.parseMapFromFile((testDir / "complete_map.json").string());
    });
}

// Test parsing map with invalid legend (not an object)
TEST_F(MapParserTest, ParseMapInvalidLegend) {
    MapParser parser(prefabManager, registry);

    std::string jsonContent = R"(
    {
        "legend": "invalid",
        "map": [
            "W W"
        ]
    })";

    createTestFile("invalid_legend.json", jsonContent);

    // Should not throw, but should log error and skip grid parsing
    EXPECT_NO_THROW({
        parser.parseMapFromFile((testDir / "invalid_legend.json").string());
    });
}

// Test parsing map with invalid map grid (not an array)
TEST_F(MapParserTest, ParseMapInvalidGrid) {
    MapParser parser(prefabManager, registry);

    std::string jsonContent = R"(
    {
        "legend": {
            "W": "wall"
        },
        "map": "invalid"
    })";

    createTestFile("invalid_grid.json", jsonContent);

    EXPECT_NO_THROW({
        parser.parseMapFromFile((testDir / "invalid_grid.json").string());
    });
}

// Test parsing map with invalid waves (not an array)
TEST_F(MapParserTest, ParseMapInvalidWaves) {
    MapParser parser(prefabManager, registry);

    std::string jsonContent = R"(
    {
        "waves": "invalid"
    })";

    createTestFile("invalid_waves.json", jsonContent);

    EXPECT_NO_THROW({
        parser.parseMapFromFile((testDir / "invalid_waves.json").string());
    });
}

// Test parsing map with wave missing required fields
TEST_F(MapParserTest, ParseMapWaveMissingFields) {
    MapParser parser(prefabManager, registry);

    std::string jsonContent = R"(
    {
        "waves": [
            {
                "posX": 5
            }
        ]
    })";

    createTestFile("wave_missing_fields.json", jsonContent);

    EXPECT_NO_THROW({
        parser.parseMapFromFile((testDir / "wave_missing_fields.json").string());
    });
}

// Test parsing map with enemy group missing required fields
TEST_F(MapParserTest, ParseMapEnemyMissingFields) {
    MapParser parser(prefabManager, registry);

    std::string jsonContent = R"(
    {
        "waves": [
            {
                "spawnLength": 10,
                "enemies": [
                    {
                        "type": "enemy1"
                    }
                ]
            }
        ]
    })";

    createTestFile("enemy_missing_fields.json", jsonContent);

    EXPECT_NO_THROW({
        parser.parseMapFromFile((testDir / "enemy_missing_fields.json").string());
    });
}

// Test parsing map with unknown prefab in legend
TEST_F(MapParserTest, ParseMapUnknownPrefabInLegend) {
    MapParser parser(prefabManager, registry);

    std::string jsonContent = R"(
    {
        "legend": {
            "U": "unknown_prefab"
        },
        "map": [
            "U"
        ]
    })";

    createTestFile("unknown_prefab_legend.json", jsonContent);

    EXPECT_NO_THROW({
        parser.parseMapFromFile((testDir / "unknown_prefab_legend.json").string());
    });
}

// Test parsing map with unknown prefab in waves
TEST_F(MapParserTest, ParseMapUnknownPrefabInWaves) {
    MapParser parser(prefabManager, registry);

    std::string jsonContent = R"(
    {
        "waves": [
            {
                "spawnLength": 5,
                "enemies": [
                    {
                        "type": "unknown_enemy",
                        "count": 1
                    }
                ]
            }
        ]
    })";

    createTestFile("unknown_prefab_waves.json", jsonContent);

    EXPECT_NO_THROW({
        parser.parseMapFromFile((testDir / "unknown_prefab_waves.json").string());
    });
}

// Test parsing map with empty background name
TEST_F(MapParserTest, ParseMapEmptyBackground) {
    MapParser parser(prefabManager, registry);

    std::string jsonContent = R"(
    {
        "background": "",
        "scrollSpeed": 1.0
    })";

    createTestFile("empty_background.json", jsonContent);

    EXPECT_NO_THROW({
        parser.parseMapFromFile((testDir / "empty_background.json").string());
    });
}

// Test parsing map with invalid map row (not a string)
TEST_F(MapParserTest, ParseMapInvalidRow) {
    MapParser parser(prefabManager, registry);

    std::string jsonContent = R"(
    {
        "legend": {
            "W": "wall"
        },
        "map": [
            123
        ]
    })";

    createTestFile("invalid_row.json", jsonContent);

    EXPECT_NO_THROW({
        parser.parseMapFromFile((testDir / "invalid_row.json").string());
    });
}

// Test parsing map with unknown token in grid
TEST_F(MapParserTest, ParseMapUnknownToken) {
    MapParser parser(prefabManager, registry);

    std::string jsonContent = R"(
    {
        "legend": {
            "W": "wall"
        },
        "map": [
            "W X W"
        ]
    })";

    createTestFile("unknown_token.json", jsonContent);

    EXPECT_NO_THROW({
        parser.parseMapFromFile((testDir / "unknown_token.json").string());
    });
}

}