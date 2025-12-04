/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** TriggerIntentComponentTests
*/

#include <gtest/gtest.h>
#include "../../../../common/components/temporary/TriggerIntentComponent.hpp"

namespace ecs {

class TriggerIntentComponentTests : public ::testing::Test {
    protected:
        void SetUp() override {
            // Setup code if needed
        }

        void TearDown() override {
            // Cleanup code if needed
        }
};

TEST_F(TriggerIntentComponentTests, DefaultConstructor) {
    TriggerIntentComponent comp;

    EXPECT_EQ(comp.getSelf(), 0);
    EXPECT_EQ(comp.getOther(), 0);
}

TEST_F(TriggerIntentComponentTests, ConstructorWithParameters) {
    Entity self = 42;
    Entity other = 24;

    TriggerIntentComponent comp(self, other);

    EXPECT_EQ(comp.getSelf(), self);
    EXPECT_EQ(comp.getOther(), other);
}

TEST_F(TriggerIntentComponentTests, SetSelf) {
    TriggerIntentComponent comp;
    Entity newSelf = 123;

    comp.setSelf(newSelf);

    EXPECT_EQ(comp.getSelf(), newSelf);
}

TEST_F(TriggerIntentComponentTests, SetOther) {
    TriggerIntentComponent comp;
    Entity newOther = 456;

    comp.setOther(newOther);

    EXPECT_EQ(comp.getOther(), newOther);
}

TEST_F(TriggerIntentComponentTests, MultipleSetters) {
    TriggerIntentComponent comp;
    Entity self1 = 10, self2 = 20;
    Entity other1 = 30, other2 = 40;

    comp.setSelf(self1);
    comp.setOther(other1);
    EXPECT_EQ(comp.getSelf(), self1);
    EXPECT_EQ(comp.getOther(), other1);

    comp.setSelf(self2);
    comp.setOther(other2);
    EXPECT_EQ(comp.getSelf(), self2);
    EXPECT_EQ(comp.getOther(), other2);
}

} // namespace ecs