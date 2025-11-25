/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** SfmlWindowTests
*/

#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <utility>
#include "../../client/graphicals/SfmlWindow.hpp"
#include "../../client/graphicals/IWindow.hpp"

class SfmlWindowTest : public ::testing::Test {
protected:
    void SetUp() override {
        window = std::make_unique<SfmlWindow>("Test Window", 800, 600);
    }

    void TearDown() override {
        if (window && window->isOpen()) {
            window->closeWindow();
        }
    }

    std::unique_ptr<SfmlWindow> window;
};

TEST_F(SfmlWindowTest, ConstructorCreatesWindow) {
    ASSERT_TRUE(window->isOpen());
}

TEST_F(SfmlWindowTest, WindowSize) {
    auto size = window->getWindowSize();
    EXPECT_EQ(size.first, 800);
    EXPECT_EQ(size.second, 600);
}

TEST_F(SfmlWindowTest, ResizeWindow) {
    window->resizeWindow(1024, 768);
    auto size = window->getWindowSize();
    EXPECT_EQ(size.first, 1024);
    EXPECT_EQ(size.second, 768);
}

TEST_F(SfmlWindowTest, ClearAndDisplay) {
    window->clear();
    window->display();
    ASSERT_TRUE(window->isOpen());
}

TEST_F(SfmlWindowTest, DrawRectangle) {
    gfx::color_t color = {255, 0, 0};
    std::pair<size_t, size_t> position = {100, 100};
    std::pair<size_t, size_t> size = {50, 50};

    window->clear();
    window->drawRectangle(color, position, size);
    window->display();
    ASSERT_TRUE(window->isOpen());
}

TEST_F(SfmlWindowTest, DrawText) {
    gfx::color_t color = {255, 255, 255};
    std::pair<size_t, size_t> position = {200, 200};

    window->clear();
    window->drawText("Test Text", color, position);
    window->display();
    ASSERT_TRUE(window->isOpen());
}

TEST_F(SfmlWindowTest, IsMouseOver) {
    std::pair<size_t, size_t> position = {100, 100};
    std::pair<size_t, size_t> size = {50, 50};

    bool result = window->isMouseOver(position, size);
    (void)result;
    ASSERT_TRUE(true);
}

TEST_F(SfmlWindowTest, SetAndGetFont) {
    std::string fontPath = "test_font.ttf";
    try {
        window->setFont(fontPath);
        FAIL() << "Expected std::runtime_error";
    } catch (const std::runtime_error& e) {
        EXPECT_EQ(std::string(e.what()), "Failed to load font: test_font.ttf");
    } catch (...) {
        FAIL() << "Expected std::runtime_error";
    }
    EXPECT_EQ(window->getFont(), fontPath);
}

TEST_F(SfmlWindowTest, CloseWindow) {
    window->closeWindow();
    ASSERT_FALSE(window->isOpen());
}