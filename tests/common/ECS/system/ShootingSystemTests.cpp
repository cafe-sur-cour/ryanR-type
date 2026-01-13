/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ShootingSystem Tests
*/

#include <gtest/gtest.h>
#include <memory>
#include "../../../../common/systems/shooting/ShootingSystem.hpp"
#include "../../../../common/ECS/entity/registry/Registry.hpp"
#include "../../../../common/components/temporary/ShootIntentComponent.hpp"
#include "../../../../common/components/permanent/ShootingStatsComponent.hpp"
#include "../../../../common/components/permanent/TransformComponent.hpp"
#include "../../../../common/components/permanent/VelocityComponent.hpp"
#include "../../../../common/Prefab/APrefab.hpp"

using namespace ecs;

class MockProjectilePrefab : public APrefab {
    public:
        ecs::Entity instantiate(const std::shared_ptr<ecs::Registry> &registry) override {
            auto entity = registry->createEntity();
            registry->addComponent(entity, std::make_shared<TransformComponent>());
            registry->addComponent(entity, std::make_shared<VelocityComponent>());
            return entity;
        }
};

/* ShootingSystem Tests */

TEST(ShootingSystemTest, SystemCreation) {
    ShootingSystem system;
    SUCCEED();
}

TEST(ShootingSystemTest, NoShootingWithoutIntent) {
    auto registry = std::make_shared<Registry>();
    auto system = std::make_shared<ShootingSystem>();

    auto entity = registry->createEntity();
    auto prefab = std::make_shared<MockProjectilePrefab>();
    registry->addComponent(entity, std::make_shared<ShootingStatsComponent>(1.0f, prefab, 300.0f));
    registry->addComponent(entity, std::make_shared<TransformComponent>(math::Vector2f(100.0f, 100.0f)));

    auto viewBefore = registry->view<TransformComponent>();
    size_t countBefore = 0;
    for (auto e : viewBefore) { (void)e; countBefore++; }

    system->updateSystem(nullptr, registry, 0.016f);

    auto viewAfter = registry->view<TransformComponent>();
    size_t countAfter = 0;
    for (auto e : viewAfter) { (void)e; countAfter++; }

    EXPECT_EQ(countAfter, countBefore);
}

TEST(ShootingSystemTest, SingleShotCreatesProjectile) {
    auto registry = std::make_shared<Registry>();
    auto system = std::make_shared<ShootingSystem>();

    auto shooter = registry->createEntity();
    auto prefab = std::make_shared<MockProjectilePrefab>();
    registry->addComponent(shooter, std::make_shared<ShootingStatsComponent>(1.0f, prefab, 300.0f));
    registry->addComponent(shooter, std::make_shared<TransformComponent>(math::Vector2f(100.0f, 100.0f)));
    registry->addComponent(shooter, std::make_shared<ShootIntentComponent>(math::Vector2f(100.0f, 100.0f)));

    auto viewBefore = registry->view<TransformComponent>();
    size_t countBefore = 0;
    for (auto e : viewBefore) { (void)e; countBefore++; }

    system->updateSystem(nullptr, registry, 0.016f);

    auto viewAfter = registry->view<TransformComponent>();
    size_t countAfter = 0;
    for (auto e : viewAfter) { (void)e; countAfter++; }

    EXPECT_EQ(countAfter, countBefore + 1);
}

TEST(ShootingSystemTest, CooldownPreventsRapidFiring) {
    auto registry = std::make_shared<Registry>();
    auto system = std::make_shared<ShootingSystem>();

    auto shooter = registry->createEntity();
    auto prefab = std::make_shared<MockProjectilePrefab>();
    registry->addComponent(shooter, std::make_shared<ShootingStatsComponent>(1.0f, prefab, 300.0f));
    registry->addComponent(shooter, std::make_shared<TransformComponent>(math::Vector2f(100.0f, 100.0f)));

    registry->addComponent(shooter, std::make_shared<ShootIntentComponent>(math::Vector2f(100.0f, 100.0f)));
    system->updateSystem(nullptr, registry, 0.016f);

    auto viewAfterFirst = registry->view<TransformComponent>();
    size_t countAfterFirstShot = 0;
    for (auto e : viewAfterFirst) { (void)e; countAfterFirstShot++; }

    registry->addComponent(shooter, std::make_shared<ShootIntentComponent>(math::Vector2f(100.0f, 100.0f)));
    system->updateSystem(nullptr, registry, 0.016f);

    auto viewAfterSecond = registry->view<TransformComponent>();
    size_t countAfterSecond = 0;
    for (auto e : viewAfterSecond) { (void)e; countAfterSecond++; }

    EXPECT_EQ(countAfterSecond, countAfterFirstShot);
}

TEST(ShootingSystemTest, CooldownAllowsShootingAfterDelay) {
    auto registry = std::make_shared<Registry>();
    auto system = std::make_shared<ShootingSystem>();

    auto shooter = registry->createEntity();
    auto prefab = std::make_shared<MockProjectilePrefab>();
    registry->addComponent(shooter, std::make_shared<ShootingStatsComponent>(2.0f, prefab, 300.0f));
    registry->addComponent(shooter, std::make_shared<TransformComponent>(math::Vector2f(100.0f, 100.0f)));

    registry->addComponent(shooter, std::make_shared<ShootIntentComponent>(math::Vector2f(100.0f, 100.0f)));
    system->updateSystem(nullptr, registry, 0.016f);

    auto viewAfterFirst = registry->view<TransformComponent>();
    size_t countAfterFirstShot = 0;
    for (auto e : viewAfterFirst) { (void)e; countAfterFirstShot++; }

    for (int i = 0; i < 32; i++) {
        system->updateSystem(nullptr, registry, 0.016f);
    }

    registry->addComponent(shooter, std::make_shared<ShootIntentComponent>(math::Vector2f(100.0f, 100.0f)));
    system->updateSystem(nullptr, registry, 0.016f);

    auto viewAfterSecond = registry->view<TransformComponent>();
    size_t countAfterSecond = 0;
    for (auto e : viewAfterSecond) { (void)e; countAfterSecond++; }

    EXPECT_GT(countAfterSecond, countAfterFirstShot);
}

TEST(ShootingSystemTest, MultiShotCreatesMultipleProjectiles) {
    auto registry = std::make_shared<Registry>();
    auto system = std::make_shared<ShootingSystem>();

    auto shooter = registry->createEntity();
    auto prefab = std::make_shared<MockProjectilePrefab>();
    MultiShotPattern tripleShot{3, 15.0f, 0.0f};
    registry->addComponent(shooter, std::make_shared<ShootingStatsComponent>(1.0f, prefab, 300.0f, tripleShot));
    registry->addComponent(shooter, std::make_shared<TransformComponent>(math::Vector2f(100.0f, 100.0f)));
    registry->addComponent(shooter, std::make_shared<ShootIntentComponent>(math::Vector2f(100.0f, 100.0f)));

    auto viewBefore = registry->view<TransformComponent>();
    size_t countBefore = 0;
    for (auto e : viewBefore) { (void)e; countBefore++; }

    system->updateSystem(nullptr, registry, 0.016f);

    auto viewAfter = registry->view<TransformComponent>();
    size_t countAfter = 0;
    for (auto e : viewAfter) { (void)e; countAfter++; }

    EXPECT_EQ(countAfter, countBefore + 3);
}

TEST(ShootingSystemTest, ProjectileHasCorrectVelocity) {
    auto registry = std::make_shared<Registry>();
    auto system = std::make_shared<ShootingSystem>();

    auto shooter = registry->createEntity();
    auto prefab = std::make_shared<MockProjectilePrefab>();
    float projectileSpeed = 400.0f;
    registry->addComponent(shooter, std::make_shared<ShootingStatsComponent>(1.0f, prefab, projectileSpeed));
    registry->addComponent(shooter, std::make_shared<TransformComponent>(math::Vector2f(100.0f, 100.0f)));
    registry->addComponent(shooter, std::make_shared<ShootIntentComponent>(math::Vector2f(100.0f, 100.0f)));

    system->updateSystem(nullptr, registry, 0.016f);

    auto allEntities = registry->view<VelocityComponent>();
    Entity projectile = 0;
    for (auto entity : allEntities) {
        if (entity != shooter) {
            projectile = entity;
        }
    }

    auto velocity = registry->getComponent<VelocityComponent>(projectile);
    ASSERT_NE(velocity, nullptr);

    math::Vector2f vel = velocity->getVelocity();
    float magnitude = std::sqrt(vel.getX() * vel.getX() + vel.getY() * vel.getY());
    EXPECT_NEAR(magnitude, projectileSpeed, 1.0f);
}

TEST(ShootingSystemTest, IntentMarkedAsProcessed) {
    auto registry = std::make_shared<Registry>();
    auto system = std::make_shared<ShootingSystem>();

    auto shooter = registry->createEntity();
    auto prefab = std::make_shared<MockProjectilePrefab>();
    registry->addComponent(shooter, std::make_shared<ShootingStatsComponent>(1.0f, prefab, 300.0f));
    registry->addComponent(shooter, std::make_shared<TransformComponent>(math::Vector2f(100.0f, 100.0f)));
    auto intent = std::make_shared<ShootIntentComponent>(math::Vector2f(100.0f, 100.0f));
    registry->addComponent(shooter, intent);

    system->updateSystem(nullptr, registry, 0.016f);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
