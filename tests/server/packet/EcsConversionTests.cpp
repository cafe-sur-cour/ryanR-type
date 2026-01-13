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

    using Lobby::convertTagComponent;
    using Lobby::convertTransformComponent;
    using Lobby::convertSpeedComponent;
    using Lobby::convertHealthComponent;
    using Lobby::convertColliderComponent;
    using Lobby::convertShootStatComponent;
    using Lobby::convertScoreComponent;
    using Lobby::convertDamageComponent;
    using Lobby::convertLifetimeComponent;
    using Lobby::convertVelocityComponent;
    using Lobby::convertControllableTagComponent;
    using Lobby::convertEnemyProjectileTagComponent;
    using Lobby::convertGameZoneColliderTagComponent;
    using Lobby::convertMobTagComponent;
    using Lobby::convertObstacleTagComponent;
    using Lobby::convertPlayerProjectileTagComponent;
    using Lobby::convertShooterTagComponent;
    using Lobby::convertProjectilePassThroughTagComponent;
    using Lobby::convertProjectilePrefabComponent;
    using Lobby::convertGameZoneComponent;
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

TEST_F(ECSConversionTest, ConvertTagComponent_WithPlayerTag) {
    registry->registerComponent<PlayerTag>();
    auto playerTag = std::make_shared<PlayerTag>();
    registry->addComponent<PlayerTag>(entity, playerTag);

    auto result = testLobby->convertTagComponent(registry, entity);

    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], static_cast<uint64_t>(PLAYER_TAG));
}

TEST_F(ECSConversionTest, ConvertTagComponent_WithoutPlayerTag) {
    auto result = testLobby->convertTagComponent(registry, entity);

    EXPECT_TRUE(result.empty());
}

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

TEST_F(ECSConversionTest, ConvertSpeedComponent_WithSpeed) {
    registry->registerComponent<SpeedComponent>();
    auto speed = std::make_shared<SpeedComponent>(15.5f);
    registry->addComponent<SpeedComponent>(entity, speed);

    auto result = testLobby->convertSpeedComponent(registry, entity);

    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], static_cast<uint64_t>(SPEED_COMP));
    EXPECT_EQ(result[1], quantization::packSpeed(15.5f));
}

TEST_F(ECSConversionTest, ConvertSpeedComponent_WithoutSpeed) {
    auto result = testLobby->convertSpeedComponent(registry, entity);

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

TEST_F(ECSConversionTest, ConvertColliderComponent_WithCollider) {
    registry->registerComponent<ColliderComponent>();
    auto collider = std::make_shared<ColliderComponent>(
        math::Vector2f(5.0f, 5.0f), math::Vector2f(10.0f, 10.0f), CollisionType::Solid);
    registry->addComponent<ColliderComponent>(entity, collider);

    auto result = testLobby->convertColliderComponent(registry, entity);

    ASSERT_EQ(result.size(), 6);
    EXPECT_EQ(result[0], static_cast<uint64_t>(COLLIDER));
    EXPECT_EQ(result[1], quantization::packPosition(5.0f));
    EXPECT_EQ(result[2], quantization::packPosition(5.0f));
    EXPECT_EQ(result[3], quantization::packSize(10.0f));
    EXPECT_EQ(result[4], quantization::packSize(10.0f));
    EXPECT_EQ(result[5], static_cast<uint64_t>(ecs::CollisionType::Solid));
}

TEST_F(ECSConversionTest, ConvertColliderComponent_WithoutCollider) {
    auto result = testLobby->convertColliderComponent(registry, entity);

    EXPECT_TRUE(result.empty());
}

TEST_F(ECSConversionTest, ConvertShootStatComponent_WithShootingStats) {
    registry->registerComponent<ShootingStatsComponent>();
    MultiShotPattern pattern{3, 30.0f, 5.0f, 180.0f};
    auto shootStats = std::make_shared<ShootingStatsComponent>(2.0f, pattern);
    shootStats->setCooldownTimer(1.5f);
    registry->addComponent<ShootingStatsComponent>(entity, shootStats);

    auto result = testLobby->convertShootStatComponent(registry, entity);

    ASSERT_EQ(result.size(), 7);
    EXPECT_EQ(result[0], static_cast<uint64_t>(SHOOTING_STATS));
    EXPECT_EQ(result[1], quantization::packTime(2.0f));
    EXPECT_EQ(result[2], quantization::packTime(1.5f));
    EXPECT_EQ(result[3], static_cast<uint64_t>(3));
    EXPECT_EQ(result[4], quantization::packAngle(30.0f));
    EXPECT_EQ(result[5], quantization::packSize(5.0f));
    EXPECT_EQ(result[6], quantization::packAngle(180.0f));
}

TEST_F(ECSConversionTest, ConvertShootStatComponent_WithoutShootingStats) {
    auto result = testLobby->convertShootStatComponent(registry, entity);

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

TEST_F(ECSConversionTest, ConvertDamageComponent_WithDamage) {
    registry->registerComponent<DamageComponent>();
    auto damage = std::make_shared<DamageComponent>(25.0f);
    registry->addComponent<DamageComponent>(entity, damage);

    auto result = testLobby->convertDamageComponent(registry, entity);

    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], static_cast<uint64_t>(DAMAGE));
    EXPECT_EQ(result[1], quantization::packDamage(25.0f));
}

TEST_F(ECSConversionTest, ConvertDamageComponent_WithoutDamage) {
    auto result = testLobby->convertDamageComponent(registry, entity);

    EXPECT_TRUE(result.empty());
}

TEST_F(ECSConversionTest, ConvertLifetimeComponent_WithLifetime) {
    registry->registerComponent<LifetimeComponent>();
    auto lifetime = std::make_shared<LifetimeComponent>(10.0f);
    registry->addComponent<LifetimeComponent>(entity, lifetime);

    auto result = testLobby->convertLifetimeComponent(registry, entity);

    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], static_cast<uint64_t>(LIFETIME));
    EXPECT_EQ(result[1], quantization::packTime(10.0f));
}

TEST_F(ECSConversionTest, ConvertLifetimeComponent_WithoutLifetime) {
    auto result = testLobby->convertLifetimeComponent(registry, entity);

    EXPECT_TRUE(result.empty());
}

TEST_F(ECSConversionTest, ConvertVelocityComponent_WithVelocity) {
    registry->registerComponent<VelocityComponent>();
    auto velocity = std::make_shared<VelocityComponent>(math::Vector2f(7.0f, -3.0f));
    registry->addComponent<VelocityComponent>(entity, velocity);

    auto result = testLobby->convertVelocityComponent(registry, entity);

    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], static_cast<uint64_t>(VELOCITY));
    EXPECT_EQ(result[1], quantization::packVelocity(7.0f));
    EXPECT_EQ(result[2], quantization::packVelocity(-3.0f));
}

TEST_F(ECSConversionTest, ConvertVelocityComponent_WithoutVelocity) {
    auto result = testLobby->convertVelocityComponent(registry, entity);

    EXPECT_TRUE(result.empty());
}

TEST_F(ECSConversionTest, ConvertControllableTagComponent_WithControllableTag) {
    registry->registerComponent<ControllableTag>();
    auto controllableTag = std::make_shared<ControllableTag>();
    registry->addComponent<ControllableTag>(entity, controllableTag);

    auto result = testLobby->convertControllableTagComponent(registry, entity);

    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], static_cast<uint64_t>(CONTROLLABLE_TAG));
}

TEST_F(ECSConversionTest, ConvertControllableTagComponent_WithoutControllableTag) {
    auto result = testLobby->convertControllableTagComponent(registry, entity);

    EXPECT_TRUE(result.empty());
}

TEST_F(ECSConversionTest, ConvertEnemyProjectileTagComponent_WithEnemyProjectileTag) {
    registry->registerComponent<EnnemyProjectileTag>();
    auto enemyProjectileTag = std::make_shared<EnnemyProjectileTag>();
    registry->addComponent<EnnemyProjectileTag>(entity, enemyProjectileTag);

    auto result = testLobby->convertEnemyProjectileTagComponent(registry, entity);

    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], static_cast<uint64_t>(ENEMY_PROJECTILE_TAG));
}

TEST_F(ECSConversionTest, ConvertEnemyProjectileTagComponent_WithoutEnemyProjectileTag) {
    auto result = testLobby->convertEnemyProjectileTagComponent(registry, entity);

    EXPECT_TRUE(result.empty());
}

TEST_F(ECSConversionTest, ConvertGameZoneColliderTagComponent_WithGameZoneColliderTag) {
    registry->registerComponent<GameZoneColliderTag>();
    auto gameZoneColliderTag = std::make_shared<GameZoneColliderTag>();
    registry->addComponent<GameZoneColliderTag>(entity, gameZoneColliderTag);

    auto result = testLobby->convertGameZoneColliderTagComponent(registry, entity);

    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], static_cast<uint64_t>(GAME_ZONE_COLLIDER_TAG));
}

TEST_F(ECSConversionTest, ConvertGameZoneColliderTagComponent_WithoutGameZoneColliderTag) {
    auto result = testLobby->convertGameZoneColliderTagComponent(registry, entity);

    EXPECT_TRUE(result.empty());
}

TEST_F(ECSConversionTest, ConvertMobTagComponent_WithMobTag) {
    registry->registerComponent<MobTag>();
    auto mobTag = std::make_shared<MobTag>();
    registry->addComponent<MobTag>(entity, mobTag);

    auto result = testLobby->convertMobTagComponent(registry, entity);

    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], static_cast<uint64_t>(MOB_TAG));
}

TEST_F(ECSConversionTest, ConvertMobTagComponent_WithoutMobTag) {
    auto result = testLobby->convertMobTagComponent(registry, entity);

    EXPECT_TRUE(result.empty());
}

TEST_F(ECSConversionTest, ConvertObstacleTagComponent_WithObstacleTag) {
    registry->registerComponent<ObstacleTag>();
    auto obstacleTag = std::make_shared<ObstacleTag>();
    registry->addComponent<ObstacleTag>(entity, obstacleTag);

    auto result = testLobby->convertObstacleTagComponent(registry, entity);

    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], static_cast<uint64_t>(OBSTACLE_TAG));
}

TEST_F(ECSConversionTest, ConvertObstacleTagComponent_WithoutObstacleTag) {
    auto result = testLobby->convertObstacleTagComponent(registry, entity);

    EXPECT_TRUE(result.empty());
}

TEST_F(ECSConversionTest, ConvertPlayerProjectileTagComponent_WithPlayerProjectileTag) {
    registry->registerComponent<PlayerProjectileTag>();
    auto playerProjectileTag = std::make_shared<PlayerProjectileTag>();
    registry->addComponent<PlayerProjectileTag>(entity, playerProjectileTag);

    auto result = testLobby->convertPlayerProjectileTagComponent(registry, entity);

    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], static_cast<uint64_t>(PLAYER_PROJECTILE_TAG));
}

TEST_F(ECSConversionTest, ConvertPlayerProjectileTagComponent_WithoutPlayerProjectileTag) {
    auto result = testLobby->convertPlayerProjectileTagComponent(registry, entity);

    EXPECT_TRUE(result.empty());
}

TEST_F(ECSConversionTest, ConvertShooterTagComponent_WithShooterTag) {
    registry->registerComponent<ShooterTag>();
    auto shooterTag = std::make_shared<ShooterTag>();
    registry->addComponent<ShooterTag>(entity, shooterTag);

    auto result = testLobby->convertShooterTagComponent(registry, entity);

    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], static_cast<uint64_t>(SHOOTER_TAG));
}

TEST_F(ECSConversionTest, ConvertShooterTagComponent_WithoutShooterTag) {
    auto result = testLobby->convertShooterTagComponent(registry, entity);

    EXPECT_TRUE(result.empty());
}

TEST_F(ECSConversionTest, ConvertProjectilePassThroughTagComponent_WithProjectilePassThroughTag) {
    registry->registerComponent<ProjectilePassThroughTag>();
    auto projectilePassThroughTag = std::make_shared<ProjectilePassThroughTag>();
    registry->addComponent<ProjectilePassThroughTag>(entity, projectilePassThroughTag);

    auto result = testLobby->convertProjectilePassThroughTagComponent(registry, entity);

    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], static_cast<uint64_t>(PROJECTILE_PASS_THROUGH_TAG));
}

TEST_F(ECSConversionTest, ConvertProjectilePassThroughTagComponent_WithoutProjectilePassThroughTag) {
    auto result = testLobby->convertProjectilePassThroughTagComponent(registry, entity);

    EXPECT_TRUE(result.empty());
}

TEST_F(ECSConversionTest, ConvertProjectilePrefabComponent_WithProjectilePrefab) {
    registry->registerComponent<ProjectilePrefabComponent>();
    auto prefabComp = std::make_shared<ProjectilePrefabComponent>("BulletPrefab");
    registry->addComponent<ProjectilePrefabComponent>(entity, prefabComp);

    auto result = testLobby->convertProjectilePrefabComponent(registry, entity);

    ASSERT_GE(result.size(), 5); // At least type + chars + terminators
    EXPECT_EQ(result[0], static_cast<uint64_t>(PROJECTILE_PREFAB));
    // Check the string encoding
    std::string reconstructed;
    size_t i = 1;
    while (i < result.size() - 3) {
        reconstructed += static_cast<char>(result[i]);
        i++;
    }
    EXPECT_EQ(reconstructed, "BulletPrefab");
    EXPECT_EQ(result[i], static_cast<uint64_t>(constants::END_OFSTRING_ST));
    EXPECT_EQ(result[i+1], static_cast<uint64_t>(constants::END_OFSTRING_ND));
    EXPECT_EQ(result[i+2], static_cast<uint64_t>(constants::END_OFSTRING_TRD));
}

TEST_F(ECSConversionTest, ConvertProjectilePrefabComponent_WithoutProjectilePrefab) {
    auto result = testLobby->convertProjectilePrefabComponent(registry, entity);

    EXPECT_TRUE(result.empty());
}

TEST_F(ECSConversionTest, ConvertGameZoneComponent_WithGameZone) {
    registry->registerComponent<GameZoneComponent>();
    math::FRect zone(0.0f, 0.0f, 800.0f, 600.0f);
    auto gameZoneComp = std::make_shared<GameZoneComponent>(zone);
    registry->addComponent<GameZoneComponent>(entity, gameZoneComp);

    auto result = testLobby->convertGameZoneComponent(registry, entity);

    ASSERT_EQ(result.size(), 5);
    EXPECT_EQ(result[0], static_cast<uint64_t>(GAME_ZONE));
    EXPECT_EQ(result[1], quantization::packSize(600.0f)); // height
    EXPECT_EQ(result[2], quantization::packSize(800.0f)); // width
    EXPECT_EQ(result[3], quantization::packPosition(0.0f));   // left
    EXPECT_EQ(result[4], quantization::packPosition(0.0f));   // top
}

TEST_F(ECSConversionTest, ConvertGameZoneComponent_WithoutGameZone) {
    auto result = testLobby->convertGameZoneComponent(registry, entity);

    EXPECT_TRUE(result.empty());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
