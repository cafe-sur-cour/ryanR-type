/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Chrono Tests
*/

#include <gtest/gtest.h>
#include <thread>
#include "../../../common/types/Chrono.hpp"

/* Chrono Tests */

TEST(ChronoTest, DefaultConstructor) {
    math::Chrono chrono;
    EXPECT_FALSE(chrono.isRunning());
    EXPECT_FLOAT_EQ(chrono.getElapsedSeconds(), 0.0f);
    EXPECT_FLOAT_EQ(chrono.getElapsedMilliseconds(), 0.0f);
}

TEST(ChronoTest, StartAndStop) {
    math::Chrono chrono;
    chrono.start();
    EXPECT_TRUE(chrono.isRunning());

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    chrono.stop();
    EXPECT_FALSE(chrono.isRunning());

    float elapsed = chrono.getElapsedSeconds();
    EXPECT_GT(elapsed, 0.05f);
    EXPECT_LT(elapsed, 0.2f); // Should be around 0.1 seconds
}

TEST(ChronoTest, Reset) {
    math::Chrono chrono;
    chrono.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    chrono.stop();

    float elapsedBeforeReset = chrono.getElapsedSeconds();
    EXPECT_GT(elapsedBeforeReset, 0.0f);

    chrono.reset();
    EXPECT_FALSE(chrono.isRunning());
    EXPECT_FLOAT_EQ(chrono.getElapsedSeconds(), 0.0f);
}

TEST(ChronoTest, GetElapsedWhileRunning) {
    math::Chrono chrono;
    chrono.start();

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    float elapsed = chrono.getElapsedSeconds();
    EXPECT_GT(elapsed, 0.02f);
    EXPECT_LT(elapsed, 0.1f);

    chrono.stop();
    float elapsedAfterStop = chrono.getElapsedSeconds();
    EXPECT_GE(elapsedAfterStop, elapsed); // Should not decrease
}

TEST(ChronoTest, MultipleStartStop) {
    math::Chrono chrono;

    chrono.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    chrono.stop();

    float firstElapsed = chrono.getElapsedSeconds();

    chrono.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(40));
    chrono.stop();

    float secondElapsed = chrono.getElapsedSeconds();

    EXPECT_GT(secondElapsed, firstElapsed);
}

TEST(ChronoTest, StartAlreadyRunning) {
    math::Chrono chrono;
    chrono.start();
    EXPECT_TRUE(chrono.isRunning());

    // Starting again should not change anything
    chrono.start();
    EXPECT_TRUE(chrono.isRunning());
}

TEST(ChronoTest, StopNotRunning) {
    math::Chrono chrono;
    EXPECT_FALSE(chrono.isRunning());

    // Stopping when not running should not crash
    chrono.stop();
    EXPECT_FALSE(chrono.isRunning());
}

TEST(ChronoTest, GetElapsedMilliseconds) {
    math::Chrono chrono;
    chrono.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    chrono.stop();

    float elapsedMs = chrono.getElapsedMilliseconds();
    EXPECT_GT(elapsedMs, 30.0f);
    EXPECT_LT(elapsedMs, 80.0f); // Should be around 50ms
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}