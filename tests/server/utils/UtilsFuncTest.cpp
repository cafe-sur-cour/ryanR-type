/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Utils Func Tests
*/

#include <gtest/gtest.h>
#include "../../server/Utils.hpp"
#include "../../../common/constants.hpp"

class UtilsTest : public ::testing::Test {
protected:
    Utils utils;
};

TEST_F(UtilsTest, HelperPrintsUsage) {
    testing::internal::CaptureStdout();
    utils.helper();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Usage:"), std::string::npos);
    EXPECT_NE(output.find("-p <port>"), std::string::npos);
    EXPECT_NE(output.find("-h"), std::string::npos);
}

TEST_F(UtilsTest, ParsCliSetsConfigCorrectly) {
    const char* argv[] = {"program", "-p", "8080", "-i", "127001"};
    int argc = sizeof(argv) / sizeof(argv[0]);
    auto config = std::make_shared<rserv::ServerConfig>();

    utils.parsCli(argc, const_cast<char**>(argv), config);

    EXPECT_EQ(config->getPort(), 8080);
    EXPECT_EQ(config->getIp(), "127001");
}

TEST_F(UtilsTest, ParsCliUsesDefaults) {
    const char* argv[] = {"program"};
    int argc = sizeof(argv) / sizeof(argv[0]);
    auto config = std::make_shared<rserv::ServerConfig>();

    utils.parsCli(argc, const_cast<char**>(argv), config);

    EXPECT_EQ(config->getPort(), constants::DEFAULT_SERVER_PORT);
    EXPECT_EQ(config->getIp(), constants::DEFAULT_SERVER_IP);
}

TEST_F(UtilsTest, ParsCliHelpExits) {
    const char* argv[] = {"program", "-h"};
    int argc = sizeof(argv) / sizeof(argv[0]);
    auto config = std::make_shared<rserv::ServerConfig>();

    testing::internal::CaptureStdout();
    EXPECT_EXIT(utils.parsCli(argc, const_cast<char**>(argv), config), ::testing::ExitedWithCode(0), "");
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Usage:"), std::string::npos);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
