/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Serializer Tests
*/

#include <gtest/gtest.h>
#include "../../../../libs/Packet/serializer/BigEndianSerialization.hpp"

class BigEndianSerializationTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up test fixtures
    }
    void TearDown() override {
        // Clean up after tests
    }
};

TEST_F(BigEndianSerializationTest, SerializeUIntProducesCorrectBytes) {
    BigEndianSerialization serializer;
    uint64_t value = 0x12345678;
    std::vector<uint8_t> expected = {0x12, 0x34, 0x56, 0x78};

    std::vector<uint8_t> result = serializer.serializeUInt(value);

    EXPECT_EQ(result, expected);
}

TEST_F(BigEndianSerializationTest, SerializeULongProducesCorrectBytes) {
    BigEndianSerialization serializer;
    uint64_t value = 0x0123456789ABCDEF;
    std::vector<uint8_t> expected = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};

    std::vector<uint8_t> result = serializer.serializeULong(value);

    EXPECT_EQ(result, expected);
}

TEST_F(BigEndianSerializationTest, SerializeUShortProducesCorrectBytes) {
    BigEndianSerialization serializer;
    uint64_t value = 0xABCD;
    std::vector<uint8_t> expected = {0xAB, 0xCD};

    std::vector<uint8_t> result = serializer.serializeUShort(value);

    EXPECT_EQ(result, expected);
}

TEST_F(BigEndianSerializationTest, SerializeUCharProducesCorrectBytes) {
    BigEndianSerialization serializer;
    uint64_t value = 0x7F;
    std::vector<uint8_t> expected = {0x7F};

    std::vector<uint8_t> result = serializer.serializeUChar(value);

    EXPECT_EQ(result, expected);
}

TEST_F(BigEndianSerializationTest, DeserializeUIntProducesCorrectValue) {
    BigEndianSerialization serializer;
    std::vector<uint8_t> data = {0x12, 0x34, 0x56, 0x78};
    uint64_t expected = 0x12345678;

    uint64_t result = serializer.deserializeUInt(data);

    EXPECT_EQ(result, expected);
}

TEST_F(BigEndianSerializationTest, DeserializeUIntWithInsufficientDataReturnsZero) {
    BigEndianSerialization serializer;
    std::vector<uint8_t> data = {0x12, 0x34};

    uint64_t result = serializer.deserializeUInt(data);

    EXPECT_EQ(result, 0);
}

TEST_F(BigEndianSerializationTest, DeserializeULongProducesCorrectValue) {
    BigEndianSerialization serializer;
    std::vector<uint8_t> data = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    uint64_t expected = 0x0123456789ABCDEF;

    uint64_t result = serializer.deserializeULong(data);

    EXPECT_EQ(result, expected);
}

TEST_F(BigEndianSerializationTest, DeserializeULongWithInsufficientDataReturnsZero) {
    BigEndianSerialization serializer;
    std::vector<uint8_t> data = {0x01, 0x23, 0x45};

    uint64_t result = serializer.deserializeULong(data);

    EXPECT_EQ(result, 0);
}

TEST_F(BigEndianSerializationTest, DeserializeUShortProducesCorrectValue) {
    BigEndianSerialization serializer;
    std::vector<uint8_t> data = {0xAB, 0xCD};
    uint64_t expected = 0xABCD;

    uint64_t result = serializer.deserializeUShort(data);

    EXPECT_EQ(result, expected);
}

TEST_F(BigEndianSerializationTest, DeserializeUCharProducesCorrectValue) {
    BigEndianSerialization serializer;
    std::vector<uint8_t> data = {0x7F};
    uint64_t expected = 0x7F;

    uint64_t result = serializer.deserializeUChar(data);

    EXPECT_EQ(result, expected);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}