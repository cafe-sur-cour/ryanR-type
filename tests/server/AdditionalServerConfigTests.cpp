/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Additional Server Tests
*/

#include <gtest/gtest.h>
#include "../../server/Server.hpp"
#include "../../server/ServerConfig.hpp"

using namespace rserv;

class AdditionalServerConfigTest : public ::testing::Test {
protected:
    ServerConfig config;
};

// Port tests
TEST_F(AdditionalServerConfigTest, SetPortToZero) {
    config.setPort(0);
    EXPECT_EQ(config.getPort(), 0);
}

TEST_F(AdditionalServerConfigTest, SetPortToMaxValue) {
    config.setPort(65535);
    EXPECT_EQ(config.getPort(), 65535);
}

TEST_F(AdditionalServerConfigTest, SetPortToCommonValues) {
    config.setPort(8080);
    EXPECT_EQ(config.getPort(), 8080);
    
    config.setPort(3000);
    EXPECT_EQ(config.getPort(), 3000);
    
    config.setPort(9090);
    EXPECT_EQ(config.getPort(), 9090);
}

// State tests
TEST_F(AdditionalServerConfigTest, SetStateToPositive) {
    config.setState(5);
    EXPECT_EQ(config.getState(), 5);
}

TEST_F(AdditionalServerConfigTest, SetStateToNegative) {
    config.setState(-10);
    EXPECT_EQ(config.getState(), -10);
}

TEST_F(AdditionalServerConfigTest, SetStateMultipleTimes) {
    config.setState(1);
    EXPECT_EQ(config.getState(), 1);
    
    config.setState(2);
    EXPECT_EQ(config.getState(), 2);
    
    config.setState(0);
    EXPECT_EQ(config.getState(), 0);
}

// Combined tests
TEST_F(AdditionalServerConfigTest, SetAllParametersAtOnce) {
    config.setPort(8080);
    config.setState(2);

    EXPECT_EQ(config.getPort(), 8080);
    EXPECT_EQ(config.getState(), 2);
}

TEST_F(AdditionalServerConfigTest, ResetAllParameters) {
    config.setPort(9090);
    config.setState(5);

    config.setPort(0);
    config.setState(-1);

    EXPECT_EQ(config.getPort(), 0);
    EXPECT_EQ(config.getState(), -1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
