/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** SfmlEventTests
*/

#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <utility>
#include "../../client/graphicals/SfmlEvent.hpp"
#include "../../client/graphicals/SfmlWindow.hpp"
#include "../../common/ECS/resourceManager/ResourceManager.hpp"

class SfmlEventTest : public ::testing::Test {
protected:
    void SetUp() override {
        window = std::make_shared<SfmlWindow>("Test Window", 800, 600);
        resourceManager = std::make_shared<ecs::ResourceManager>();
        eventHandler = std::make_unique<SfmlEvent>(resourceManager, window);
    }

    void TearDown() override {
        if (window && window->isOpen()) {
            window->closeWindow();
        }
    }

    std::shared_ptr<SfmlWindow> window;
    std::shared_ptr<ecs::ResourceManager> resourceManager;
    std::unique_ptr<SfmlEvent> eventHandler;
};

TEST_F(SfmlEventTest, ConstructorCreatesEventHandler) {
    ASSERT_TRUE(eventHandler != nullptr);
}

TEST_F(SfmlEventTest, PollEventsReturnsNothingWhenNoEvents) {
    std::pair<int, int> gridSize = {10, 10};
    auto event = eventHandler->pollEvents(gridSize);
    EXPECT_EQ(event, gfx::IEvent::NOTHING);
}

TEST_F(SfmlEventTest, GetMousePos) {
    auto pos = eventHandler->getMousePos();
    (void)pos;
    ASSERT_TRUE(true);
}

TEST_F(SfmlEventTest, IsKeyPressed) {
    bool pressed = eventHandler->isKeyPressed(gfx::IEvent::UP);
    EXPECT_FALSE(pressed);
}

TEST_F(SfmlEventTest, IsMouseButtonPressed) {
    bool leftPressed = eventHandler->isMouseButtonPressed(0);
    bool rightPressed = eventHandler->isMouseButtonPressed(1);
    EXPECT_FALSE(leftPressed);
    EXPECT_FALSE(rightPressed);
}

TEST_F(SfmlEventTest, GetTypedTextInitiallyEmpty) {
    std::string text = eventHandler->getTypedText();
    EXPECT_TRUE(text.empty());
}

TEST_F(SfmlEventTest, CleanupDoesNotCrash) {
    eventHandler->cleanup();
    ASSERT_TRUE(true);
}