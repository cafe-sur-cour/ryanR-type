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

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    chrono.stop();
    EXPECT_FALSE(chrono.isRunning());

    float elapsed = chrono.getElapsedSeconds();
    EXPECT_GT(elapsed, 0.0f);
    EXPECT_LT(elapsed, 1.0f); // Should be around 0.01 seconds
}

TEST(ChronoTest, Reset) {
    math::Chrono chrono;
    chrono.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
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

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    float elapsed = chrono.getElapsedSeconds();
    EXPECT_GT(elapsed, 0.0f);
    EXPECT_LT(elapsed, 1.0f);

    chrono.stop();
    float elapsedAfterStop = chrono.getElapsedSeconds();
    EXPECT_GE(elapsedAfterStop, elapsed); // Should not decrease
}

TEST(ChronoTest, MultipleStartStop) {
    math::Chrono chrono;

    chrono.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    chrono.stop();

    float firstElapsed = chrono.getElapsedSeconds();

    chrono.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
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
    std::this_thread::sleep_for(std::chrono::milliseconds(15));
    chrono.stop();

    float elapsedMs = chrono.getElapsedMilliseconds();
    EXPECT_GT(elapsedMs, 10.0f);
    EXPECT_LT(elapsedMs, 50.0f); // Should be around 15ms
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}