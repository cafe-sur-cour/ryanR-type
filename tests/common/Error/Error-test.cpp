/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Error tests
*/

#include <gtest/gtest.h>
#include <string>
#include "ServerErrror.hpp"

namespace err {

class ServerErrorTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }

    void TearDown() override {
        // Cleanup code if needed
    }
};

TEST_F(ServerErrorTest, DefaultConstructor) {
    ServerError error("Test message");

    EXPECT_EQ(error.getCode(), static_cast<int>(ServerError::ErrorCode::UNKNOWN));
    EXPECT_STREQ(error.what(), "Test message");
    EXPECT_EQ(error.getType(), "ServerError");
}

TEST_F(ServerErrorTest, ConstructorWithCode) {
    ServerError error("Connection failed", ServerError::ErrorCode::CONNECTION_FAILED);

    EXPECT_EQ(error.getCode(), static_cast<int>(ServerError::ErrorCode::CONNECTION_FAILED));
    EXPECT_STREQ(error.what(), "Connection failed");
    EXPECT_EQ(error.getType(), "ServerError");
}

TEST_F(ServerErrorTest, GetDetailsFormat) {
    ServerError error("Test error", ServerError::ErrorCode::TIMEOUT);

    std::string expected = "[ServerError] Code: 1002 - Test error";
    EXPECT_EQ(error.getDetails(), expected);
}

TEST_F(ServerErrorTest, ErrorCodes) {
    EXPECT_EQ(static_cast<int>(ServerError::ErrorCode::UNKNOWN), 1000);
    EXPECT_EQ(static_cast<int>(ServerError::ErrorCode::CONNECTION_FAILED), 1001);
    EXPECT_EQ(static_cast<int>(ServerError::ErrorCode::TIMEOUT), 1002);
    EXPECT_EQ(static_cast<int>(ServerError::ErrorCode::INVALID_REQUEST), 1003);
    EXPECT_EQ(static_cast<int>(ServerError::ErrorCode::INTERNAL_ERROR), 1004);
    EXPECT_EQ(static_cast<int>(ServerError::ErrorCode::LIBRARY_LOAD_FAILED), 1005);
    EXPECT_EQ(static_cast<int>(ServerError::ErrorCode::CONFIG_ERROR), 1006);
}

TEST_F(ServerErrorTest, InheritsFromStdException) {
    ServerError error("Exception test");

    // Test that it can be caught as std::exception
    try {
        throw error;
    } catch (const std::exception& e) {
        EXPECT_STREQ(e.what(), "Exception test");
    }
}

TEST_F(ServerErrorTest, InheritsFromIError) {
    ServerError error("Interface test", ServerError::ErrorCode::INTERNAL_ERROR);

    IError* iError = &error;
    EXPECT_EQ(iError->getCode(), static_cast<int>(ServerError::ErrorCode::INTERNAL_ERROR));
    EXPECT_STREQ(iError->what(), "Interface test");
    EXPECT_EQ(iError->getType(), "ServerError");
    EXPECT_EQ(iError->getDetails(), "[ServerError] Code: 1004 - Interface test");
}

} // namespace err
