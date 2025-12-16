/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Server Config Tests
*/

#include <gtest/gtest.h>
#include "../../server/Server.hpp"
#include "../../server/ServerConfig.hpp"

using namespace rserv;

/* ServerConfig class Tests */

class ServerConfigTest : public ::testing::Test {
protected:
    ServerConfig config;
};

TEST_F(ServerConfigTest, ConstructorSetsPort) {
    EXPECT_EQ(config.getPort(), 4242);
}

TEST_F(ServerConfigTest, InitialState) {
    EXPECT_EQ(config.getState(), -1);
}

TEST_F(ServerConfigTest, SetAndGetState) {
    config.setState(1);
    EXPECT_EQ(config.getState(), 1);
}

TEST_F(ServerConfigTest, SetAndGetPort) {
    config.setPort(9090);
    EXPECT_EQ(config.getPort(), 9090);
}

/* Server class Tests (about config) */
