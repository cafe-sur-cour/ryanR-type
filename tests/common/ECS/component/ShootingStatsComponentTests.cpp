/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ShootingStatsComponent Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/components/permanent/ShootingStatsComponent.hpp"
#include "../../../../common/Prefab/APrefab.hpp"

using namespace ecs;

// Mock prefab for testing
class MockProjectilePrefab : public APrefab {
    public:
        ecs::Entity instantiate(const std::shared_ptr<ecs::Registry> &registry) override {
            return registry->createEntity();
        }
};

/* ShootingStatsComponent Tests */

TEST(ShootingStatsComponentTest, DefaultConstructor) {
    ShootingStatsComponent comp;
    EXPECT_EQ(comp.getFireRate(), 1.0f);
    EXPECT_EQ(comp.getProjectilePrefab(), nullptr);
    EXPECT_EQ(comp.getProjectileSpeed(), 300.0f);
    EXPECT_EQ(comp.getCooldownTimer(), 0.0f);
    EXPECT_TRUE(comp.canShoot());
}

TEST(ShootingStatsComponentTest, ParameterizedConstructor) {
    auto prefab = std::make_shared<MockProjectilePrefab>();
    ShootingStatsComponent comp(2.0f, prefab, 500.0f);
    EXPECT_EQ(comp.getFireRate(), 2.0f);
    EXPECT_EQ(comp.getProjectilePrefab(), prefab);
    EXPECT_EQ(comp.getProjectileSpeed(), 500.0f);
}

TEST(ShootingStatsComponentTest, GettersAndSetters) {
    ShootingStatsComponent comp;
    auto prefab = std::make_shared<MockProjectilePrefab>();
    
    comp.setFireRate(3.0f);
    comp.setProjectilePrefab(prefab);
    comp.setProjectileSpeed(800.0f);
    
    EXPECT_EQ(comp.getFireRate(), 3.0f);
    EXPECT_EQ(comp.getProjectilePrefab(), prefab);
    EXPECT_EQ(comp.getProjectileSpeed(), 800.0f);
}

TEST(ShootingStatsComponentTest, CooldownMechanism) {
    ShootingStatsComponent comp(2.0f);
    EXPECT_TRUE(comp.canShoot());
    
    comp.resetCooldown();
    EXPECT_FLOAT_EQ(comp.getCooldownTimer(), 0.5f);
    EXPECT_FALSE(comp.canShoot());
    
    comp.updateCooldown(0.3f);
    EXPECT_FLOAT_EQ(comp.getCooldownTimer(), 0.2f);
    EXPECT_FALSE(comp.canShoot());
    
    comp.updateCooldown(0.3f);
    EXPECT_FLOAT_EQ(comp.getCooldownTimer(), -0.1f);
    EXPECT_TRUE(comp.canShoot());
}

TEST(ShootingStatsComponentTest, MultiShotPatternDefault) {
    ShootingStatsComponent comp;
    MultiShotPattern pattern = comp.getMultiShotPattern();
    EXPECT_EQ(pattern.shotCount, 1);
    EXPECT_FLOAT_EQ(pattern.angleSpread, 0.0f);
    EXPECT_FLOAT_EQ(pattern.offsetDistance, 0.0f);
}

TEST(ShootingStatsComponentTest, MultiShotPatternCustom) {
    MultiShotPattern pattern(3, 15.0f, 10.0f);
    auto prefab = std::make_shared<MockProjectilePrefab>();
    ShootingStatsComponent comp(1.0f, prefab, 300.0f, pattern);
    
    MultiShotPattern retrieved = comp.getMultiShotPattern();
    EXPECT_EQ(retrieved.shotCount, 3);
    EXPECT_FLOAT_EQ(retrieved.angleSpread, 15.0f);
    EXPECT_FLOAT_EQ(retrieved.offsetDistance, 10.0f);
}

TEST(ShootingStatsComponentTest, SetMultiShotPattern) {
    ShootingStatsComponent comp;
    MultiShotPattern newPattern(5, 30.0f, 20.0f);
    comp.setMultiShotPattern(newPattern);
    
    MultiShotPattern retrieved = comp.getMultiShotPattern();
    EXPECT_EQ(retrieved.shotCount, 5);
    EXPECT_FLOAT_EQ(retrieved.angleSpread, 30.0f);
    EXPECT_FLOAT_EQ(retrieved.offsetDistance, 20.0f);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
