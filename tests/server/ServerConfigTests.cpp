/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Server Config Tests
*/

#include <gtest/gtest.h>
#include "../../server/Server.hpp"
#include "../../server/ServerConfig.hpp"

/* ServerConfig class Tests */

class ServerConfigTest : public ::testing::Test {
protected:
    ServerConfig config{8080};
};

TEST_F(ServerConfigTest, ConstructorSetsPort) {
    EXPECT_EQ(config.getPort(), 8080);
}

TEST_F(ServerConfigTest, InitialState) {
    EXPECT_EQ(config.getState(), -1);
}

TEST_F(ServerConfigTest, InitialFd) {
    EXPECT_EQ(config.getFd(), -1);
}

TEST_F(ServerConfigTest, SetAndGetState) {
    config.setState(1);
    EXPECT_EQ(config.getState(), 1);
}

TEST_F(ServerConfigTest, SetAndGetFd) {
    config.setFd(5);
    EXPECT_EQ(config.getFd(), 5);
}

TEST_F(ServerConfigTest, SetAndGetPort) {
    config.setPort(9090);
    EXPECT_EQ(config.getPort(), 9090);
}

/* Server class Tests (about config) */

class ServerTest : public ::testing::Test {
protected:
    Server server{8080};
};

TEST_F(ServerTest, Constructor) {
    EXPECT_EQ(server.getPort(), 8080);
    EXPECT_EQ(server.getState(), -1);
}

TEST_F(ServerTest, Init) {
    server.init();
    EXPECT_EQ(server.getState(), 0);
}

TEST_F(ServerTest, Start) {
    server.start();
    EXPECT_EQ(server.getState(), 1);
}

TEST_F(ServerTest, Stop) {
    server.start();
    server.stop();
    EXPECT_EQ(server.getState(), 0);
}

TEST_F(ServerTest, OperatorInt) {
    server.init();
    EXPECT_EQ(static_cast<int>(server), 0);
    server.start();
    EXPECT_EQ(static_cast<int>(server), 1);
}

TEST_F(ServerTest, GetConfig) {
    auto conf = server.getConfig();
    ASSERT_NE(conf, nullptr);
    EXPECT_EQ(conf->getPort(), 8080);
}

TEST_F(ServerTest, GetFd) {
    server.getConfig()->setFd(10);
    EXPECT_EQ(server.getFd(), 10);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}