/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** DeathIntentComponent Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/components/temporary/DeathIntentComponent.hpp"

using namespace ecs;

/* DeathIntentComponent Tests */

TEST(DeathIntentComponentTest, DefaultConstructor) {
    DeathIntentComponent comp;
    // DeathIntentComponent is a marker component with no data
    EXPECT_TRUE(true); // Just ensure it can be instantiated
}

TEST(DeathIntentComponentTest, CopyConstructor) {
    DeathIntentComponent comp1;
    DeathIntentComponent comp2(comp1);

    (void)comp2; // To avoid unused variable warning
    EXPECT_TRUE(true); // Marker component, no data to check
}

TEST(DeathIntentComponentTest, AssignmentOperator) {
    DeathIntentComponent comp1;
    DeathIntentComponent comp2;
    comp2 = comp1;
    EXPECT_TRUE(true); // Marker component, no data to check
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}