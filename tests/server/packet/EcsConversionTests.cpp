/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ECS Conversion Tests
*/

#include <gtest/gtest.h>
#include <memory>
#include <vector>
#include "../../server/Lobby.hpp"
#include "../../common/ECS/entity/registry/Registry.hpp"
#include "../../common/components/tags/PlayerTag.hpp"
#include "../../common/components/permanent/TransformComponent.hpp"
#include "../../common/components/permanent/SpeedComponent.hpp"
#include "../../common/components/permanent/HealthComponent.hpp"
#include "../../common/components/permanent/ColliderComponent.hpp"
#include "../../common/components/permanent/ShootingStatsComponent.hpp"
#include "../../common/components/permanent/ScoreComponent.hpp"
#include "../../common/components/permanent/DamageComponent.hpp"
#include "../../common/components/permanent/LifetimeComponent.hpp"
#include "../../common/components/permanent/VelocityComponent.hpp"
#include "../../common/components/tags/ControllableTag.hpp"
#include "../../common/components/tags/EnnemyProjectileTag.hpp"
#include "../../common/components/tags/GameZoneColliderTag.hpp"
#include "../../common/components/tags/MobTag.hpp"
#include "../../common/components/tags/ObstacleTag.hpp"
#include "../../common/components/tags/PlayerProjectileTag.hpp"
#include "../../common/components/tags/ShooterTag.hpp"
#include "../../common/components/tags/ProjectilePassThroughTag.hpp"
#include "../../common/components/permanent/ProjectilePrefabComponent.hpp"
#include "../../common/components/permanent/GameZoneComponent.hpp"
#include "../../common/translationToECS.hpp"
#include "../../common/types/Vector2f.hpp"
#include "../../common/types/FRect.hpp"
#include "../../common/FloatQuantization.hpp"

using namespace rserv;
using namespace ecs;

class TestLobby : public Lobby {
public:
    TestLobby() : Lobby(nullptr, {}, "test", true, 0) {}

    using Lobby::convertTransformComponent;
    using Lobby::convertHealthComponent;
    using Lobby::convertScoreComponent;
};

class ECSConversionTest : public ::testing::Test {
protected:
    void SetUp() override {
        registry = std::make_shared<Registry>();
        testLobby = std::make_unique<TestLobby>();
    }

    std::shared_ptr<Registry> registry;
    std::unique_ptr<TestLobby> testLobby;
    Entity entity = 1;
};

TEST_F(ECSConversionTest, ConvertTransformComponent_WithTransform) {
    registry->registerComponent<TransformComponent>();
    auto transform = std::make_shared<TransformComponent>(
        math::Vector2f(10.0f, 20.0f), 45.0f, math::Vector2f(2.0f, 3.0f));
    registry->addComponent<TransformComponent>(entity, transform);

    auto result = testLobby->convertTransformComponent(registry, entity);

    ASSERT_EQ(result.size(), 6);
    EXPECT_EQ(result[0], static_cast<uint64_t>(TRANSFORM));
    EXPECT_EQ(result[1], quantization::packPosition(10.0f));
    EXPECT_EQ(result[2], quantization::packPosition(20.0f));
    EXPECT_EQ(result[3], quantization::packRotation(45.0f));
    EXPECT_EQ(result[4], quantization::packScale(2.0f));
    EXPECT_EQ(result[5], quantization::packScale(3.0f));
}

TEST_F(ECSConversionTest, ConvertTransformComponent_WithoutTransform) {
    auto result = testLobby->convertTransformComponent(registry, entity);

    EXPECT_TRUE(result.empty());
}

TEST_F(ECSConversionTest, ConvertHealthComponent_WithHealth) {
    registry->registerComponent<HealthComponent>();
    auto health = std::make_shared<HealthComponent>(100.0f);
    health->setBaseHealth(150.0f);
    registry->addComponent<HealthComponent>(entity, health);

    auto result = testLobby->convertHealthComponent(registry, entity);

    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], static_cast<uint64_t>(HEALTH));
    EXPECT_EQ(result[1], quantization::packHealth(100.0f));
    EXPECT_EQ(result[2], quantization::packHealth(150.0f));
}

TEST_F(ECSConversionTest, ConvertHealthComponent_WithoutHealth) {
    auto result = testLobby->convertHealthComponent(registry, entity);

    EXPECT_TRUE(result.empty());
}

TEST_F(ECSConversionTest, ConvertScoreComponent_WithScore) {
    registry->registerComponent<ScoreComponent>();
    auto score = std::make_shared<ScoreComponent>(2500);
    registry->addComponent<ScoreComponent>(entity, score);

    auto result = testLobby->convertScoreComponent(registry, entity);

    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], static_cast<uint64_t>(SCORE));
    EXPECT_EQ(result[1], static_cast<uint64_t>(2500));
}

TEST_F(ECSConversionTest, ConvertScoreComponent_WithoutScore) {
    auto result = testLobby->convertScoreComponent(registry, entity);

    EXPECT_TRUE(result.empty());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
