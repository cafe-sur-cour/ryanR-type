/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SpatialGrid Tests
*/

#include <gtest/gtest.h>
#include <algorithm>
#include <set>
#include <cmath>
#include "SpatialGrid/SpatialGrid.hpp"

TEST(SpatialGridTest, DefaultConstructor) {
    ecs::SpatialGrid grid;
    EXPECT_FLOAT_EQ(grid.getCellSize(), 128.0f);
    EXPECT_EQ(grid.getNumCols(), static_cast<size_t>(std::ceil((1920.0f + 2 * 200.0f) / 128.0f)) + 1);
    EXPECT_EQ(grid.getNumRows(), static_cast<size_t>(std::ceil((1080.0f + 2 * 200.0f) / 128.0f)) + 1);
}

TEST(SpatialGridTest, CustomConstructor) {
    ecs::SpatialGrid grid(800.0f, 600.0f, 64.0f, 100.0f);
    EXPECT_FLOAT_EQ(grid.getCellSize(), 64.0f);
    EXPECT_EQ(grid.getNumCols(), static_cast<size_t>(std::ceil((800.0f + 2 * 100.0f) / 64.0f)) + 1);
    EXPECT_EQ(grid.getNumRows(), static_cast<size_t>(std::ceil((600.0f + 2 * 100.0f) / 64.0f)) + 1);
}

TEST(SpatialGridTest, Clear) {
    ecs::SpatialGrid grid(400.0f, 400.0f, 100.0f);
    math::FRect bounds(0.0f, 0.0f, 50.0f, 50.0f);
    grid.insert(1, bounds);
    grid.insert(2, bounds);

    auto entities = grid.query(bounds);
    EXPECT_EQ(entities.size(), 2);

    grid.clear();
    entities = grid.query(bounds);
    EXPECT_EQ(entities.size(), 0);
}

TEST(SpatialGridTest, InsertAndQuerySingleEntity) {
    ecs::SpatialGrid grid(400.0f, 400.0f, 100.0f);
    math::FRect bounds(10.0f, 10.0f, 50.0f, 50.0f);
    grid.insert(1, bounds);

    auto entities = grid.query(bounds);
    EXPECT_EQ(entities.size(), 1);
    EXPECT_EQ(entities[0], 1);
}

TEST(SpatialGridTest, InsertAndQueryMultipleEntities) {
    ecs::SpatialGrid grid(400.0f, 400.0f, 100.0f);
    math::FRect bounds1(10.0f, 10.0f, 50.0f, 50.0f);
    math::FRect bounds2(60.0f, 60.0f, 50.0f, 50.0f);
    math::FRect bounds3(120.0f, 120.0f, 50.0f, 50.0f);

    grid.insert(1, bounds1);
    grid.insert(2, bounds2);
    grid.insert(3, bounds3);

    auto entities = grid.query(math::FRect(0.0f, 0.0f, 200.0f, 200.0f));
    EXPECT_EQ(entities.size(), 3);
    EXPECT_TRUE(std::find(entities.begin(), entities.end(), static_cast<size_t>(1)) != entities.end());
    EXPECT_TRUE(std::find(entities.begin(), entities.end(), static_cast<size_t>(2)) != entities.end());
    EXPECT_TRUE(std::find(entities.begin(), entities.end(), static_cast<size_t>(3)) != entities.end());
}

TEST(SpatialGridTest, QueryNonOverlappingEntities) {
    ecs::SpatialGrid grid(400.0f, 400.0f, 100.0f);
    math::FRect bounds1(10.0f, 10.0f, 50.0f, 50.0f);
    math::FRect bounds2(200.0f, 200.0f, 50.0f, 50.0f);

    grid.insert(1, bounds1);
    grid.insert(2, bounds2);

    auto entities = grid.query(bounds1);
    EXPECT_EQ(entities.size(), 1);
    EXPECT_EQ(entities[0], 1);

    entities = grid.query(bounds2);
    EXPECT_EQ(entities.size(), 1);
    EXPECT_EQ(entities[0], 2);
}

TEST(SpatialGridTest, QueryOverlappingEntities) {
    ecs::SpatialGrid grid(400.0f, 400.0f, 100.0f);
    math::FRect bounds1(10.0f, 10.0f, 50.0f, 50.0f);
    math::FRect bounds2(30.0f, 30.0f, 50.0f, 50.0f);

    grid.insert(1, bounds1);
    grid.insert(2, bounds2);

    auto entities = grid.query(bounds1);
    EXPECT_EQ(entities.size(), 2);
    EXPECT_TRUE(std::find(entities.begin(), entities.end(), static_cast<size_t>(1)) != entities.end());
    EXPECT_TRUE(std::find(entities.begin(), entities.end(), static_cast<size_t>(2)) != entities.end());
}

TEST(SpatialGridTest, EntitySpanningMultipleCells) {
    ecs::SpatialGrid grid(400.0f, 400.0f, 100.0f);
    math::FRect bounds(50.0f, 50.0f, 150.0f, 150.0f);

    grid.insert(1, bounds);

    auto entities = grid.query(bounds);
    EXPECT_EQ(entities.size(), 1);
    EXPECT_EQ(entities[0], 1);
}

TEST(SpatialGridTest, GetPotentialPairsNoPairs) {
    ecs::SpatialGrid grid(400.0f, 400.0f, 100.0f);
    math::FRect bounds1(10.0f, 10.0f, 50.0f, 50.0f);
    math::FRect bounds2(200.0f, 200.0f, 50.0f, 50.0f);

    grid.insert(1, bounds1);
    grid.insert(2, bounds2);

    auto pairs = grid.getPotentialPairs();
    EXPECT_EQ(pairs.size(), 0);
}

TEST(SpatialGridTest, GetPotentialPairsSingleCell) {
    ecs::SpatialGrid grid(400.0f, 400.0f, 100.0f);
    math::FRect bounds(10.0f, 10.0f, 50.0f, 50.0f);

    grid.insert(1, bounds);
    grid.insert(2, bounds);
    grid.insert(3, bounds);

    auto pairs = grid.getPotentialPairs();
    EXPECT_EQ(pairs.size(), 3); // C(3,2) = 3 pairs

    // Check that all pairs are present
    std::set<std::pair<size_t, size_t>> expectedPairs = {{1,2}, {1,3}, {2,3}};
    std::set<std::pair<size_t, size_t>> actualPairs;
    for (const auto& p : pairs) {
        actualPairs.insert({std::min(p.first, p.second), std::max(p.first, p.second)});
    }
    EXPECT_EQ(actualPairs, expectedPairs);
}

TEST(SpatialGridTest, GetPotentialPairsMultipleCells) {
    ecs::SpatialGrid grid(400.0f, 400.0f, 100.0f);
    math::FRect bounds1(10.0f, 10.0f, 50.0f, 50.0f);
    math::FRect bounds2(30.0f, 30.0f, 50.0f, 50.0f); // Overlaps with bounds1
    math::FRect bounds3(200.0f, 200.0f, 50.0f, 50.0f); // Separate cell

    grid.insert(1, bounds1);
    grid.insert(2, bounds2);
    grid.insert(3, bounds3);

    auto pairs = grid.getPotentialPairs();
    EXPECT_EQ(pairs.size(), 1); // Only one pair in the overlapping cell
    EXPECT_EQ(pairs[0], std::make_pair(1, 2));
}

TEST(SpatialGridTest, SetCellSize) {
    ecs::SpatialGrid grid(400.0f, 400.0f, 100.0f);
    EXPECT_FLOAT_EQ(grid.getCellSize(), 100.0f);

    grid.setCellSize(50.0f);
    EXPECT_FLOAT_EQ(grid.getCellSize(), 50.0f);
    EXPECT_EQ(grid.getNumCols(), static_cast<size_t>(std::ceil((400.0f + 2 * 200.0f) / 50.0f)) + 1);
    EXPECT_EQ(grid.getNumRows(), static_cast<size_t>(std::ceil((400.0f + 2 * 200.0f) / 50.0f)) + 1);
}

TEST(SpatialGridTest, OutOfBoundsQuery) {
    ecs::SpatialGrid grid(400.0f, 400.0f, 100.0f);
    math::FRect bounds(10.0f, 10.0f, 50.0f, 50.0f);
    grid.insert(1, bounds);

    // Query outside the world bounds
    math::FRect queryBounds(1000.0f, 1000.0f, 50.0f, 50.0f);
    auto entities = grid.query(queryBounds);
    EXPECT_EQ(entities.size(), 0);
}

TEST(SpatialGridTest, NegativeCoordinates) {
    ecs::SpatialGrid grid(400.0f, 400.0f, 100.0f, 200.0f); // Padding allows negative coords
    math::FRect bounds(-50.0f, -50.0f, 50.0f, 50.0f);
    grid.insert(1, bounds);

    auto entities = grid.query(bounds);
    EXPECT_EQ(entities.size(), 1);
    EXPECT_EQ(entities[0], 1);
}

TEST(SpatialGridTest, DuplicateEntityInsertion) {
    ecs::SpatialGrid grid(400.0f, 400.0f, 100.0f);
    math::FRect bounds(10.0f, 10.0f, 50.0f, 50.0f);

    grid.insert(1, bounds);
    grid.insert(1, bounds); // Insert same entity again

    auto entities = grid.query(bounds);
    EXPECT_EQ(entities.size(), 1); // Should not duplicate
    EXPECT_EQ(entities[0], 1);
}

TEST(SpatialGridTest, LargeWorld) {
    ecs::SpatialGrid grid(10000.0f, 10000.0f, 500.0f);
    math::FRect bounds(1000.0f, 1000.0f, 100.0f, 100.0f);
    grid.insert(1, bounds);

    auto entities = grid.query(bounds);
    EXPECT_EQ(entities.size(), 1);
    EXPECT_EQ(entities[0], 1);
}