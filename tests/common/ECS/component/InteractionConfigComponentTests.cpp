/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** InteractionConfigComponentTests
*/

#include <gtest/gtest.h>
#include "../../../../common/components/permanent/InteractionConfigComponent.hpp"

namespace ecs {

class InteractionConfigComponentTests : public ::testing::Test {
    protected:
        void SetUp() override {
            // Setup code if needed
        }

        void TearDown() override {
            // Cleanup code if needed
        }
};

TEST_F(InteractionConfigComponentTests, DefaultConstructor) {
    InteractionConfigComponent comp;

    EXPECT_TRUE(comp.getMappings().empty());
}

TEST_F(InteractionConfigComponentTests, ConstructorWithMappings) {
    std::vector<InteractionMapping> mappings = {
        {{"enemy", "player"}, "damage", "score"},
        {{"powerup"}, "collect", ""}
    };

    InteractionConfigComponent comp(mappings);

    EXPECT_EQ(comp.getMappings().size(), 2);
    EXPECT_EQ(comp.getMappings()[0].targetTags, std::vector<std::string>({"enemy", "player"}));
    EXPECT_EQ(comp.getMappings()[0].actionToOther, "damage");
    EXPECT_EQ(comp.getMappings()[0].actionToSelf, "score");
    EXPECT_EQ(comp.getMappings()[1].targetTags, std::vector<std::string>({"powerup"}));
    EXPECT_EQ(comp.getMappings()[1].actionToOther, "collect");
    EXPECT_EQ(comp.getMappings()[1].actionToSelf, "");
}

TEST_F(InteractionConfigComponentTests, SetMappings) {
    InteractionConfigComponent comp;
    std::vector<InteractionMapping> mappings = {
        {{"test"}, "action1", "action2"}
    };

    comp.setMappings(mappings);

    EXPECT_EQ(comp.getMappings().size(), 1);
    EXPECT_EQ(comp.getMappings()[0].targetTags, std::vector<std::string>({"test"}));
    EXPECT_EQ(comp.getMappings()[0].actionToOther, "action1");
    EXPECT_EQ(comp.getMappings()[0].actionToSelf, "action2");
}

TEST_F(InteractionConfigComponentTests, AddMapping) {
    InteractionConfigComponent comp;

    InteractionMapping mapping1 = {{"enemy"}, "damage", ""};
    InteractionMapping mapping2 = {{"player"}, "", "heal"};

    comp.addMapping(mapping1);
    EXPECT_EQ(comp.getMappings().size(), 1);

    comp.addMapping(mapping2);
    EXPECT_EQ(comp.getMappings().size(), 2);

    EXPECT_EQ(comp.getMappings()[0].targetTags, std::vector<std::string>({"enemy"}));
    EXPECT_EQ(comp.getMappings()[0].actionToOther, "damage");
    EXPECT_EQ(comp.getMappings()[0].actionToSelf, "");

    EXPECT_EQ(comp.getMappings()[1].targetTags, std::vector<std::string>({"player"}));
    EXPECT_EQ(comp.getMappings()[1].actionToOther, "");
    EXPECT_EQ(comp.getMappings()[1].actionToSelf, "heal");
}

TEST_F(InteractionConfigComponentTests, EmptyActions) {
    InteractionConfigComponent comp;
    InteractionMapping mapping = {{"neutral"}, "", ""};

    comp.addMapping(mapping);

    EXPECT_EQ(comp.getMappings()[0].actionToOther, "");
    EXPECT_EQ(comp.getMappings()[0].actionToSelf, "");
}

TEST_F(InteractionConfigComponentTests, MultipleTargetTags) {
    InteractionConfigComponent comp;
    InteractionMapping mapping = {{"tag1", "tag2", "tag3"}, "complex_action", "response_action"};

    comp.addMapping(mapping);

    EXPECT_EQ(comp.getMappings()[0].targetTags.size(), 3);
    EXPECT_EQ(comp.getMappings()[0].targetTags[0], "tag1");
    EXPECT_EQ(comp.getMappings()[0].targetTags[1], "tag2");
    EXPECT_EQ(comp.getMappings()[0].targetTags[2], "tag3");
    EXPECT_EQ(comp.getMappings()[0].actionToOther, "complex_action");
    EXPECT_EQ(comp.getMappings()[0].actionToSelf, "response_action");
}

TEST_F(InteractionConfigComponentTests, GetMappingsReturnsConstReference) {
    InteractionConfigComponent comp;
    InteractionMapping mapping = {{"test"}, "action", ""};
    comp.addMapping(mapping);

    const auto& mappings = comp.getMappings();
    EXPECT_EQ(mappings.size(), 1);

    // Verify we can't modify through the reference
    // mappings.push_back({}); // This should not compile
}

} // namespace ecs