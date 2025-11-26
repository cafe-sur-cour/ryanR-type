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

TEST_F(BigEndianSerializationTest, SerializeIntProducesCorrectBytes) {
    BigEndianSerialization serializer;
    uint64_t value = 0x12345678;
    std::vector<uint8_t> expected = {0x12, 0x34, 0x56, 0x78};

    std::vector<uint8_t> result = serializer.serializeInt(value);

    EXPECT_EQ(result, expected);
}

TEST_F(BigEndianSerializationTest, SerializeLongProducesCorrectBytes) {
    BigEndianSerialization serializer;
    uint64_t value = 0x0123456789ABCDEF;
    std::vector<uint8_t> expected = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};

    std::vector<uint8_t> result = serializer.serializeLong(value);

    EXPECT_EQ(result, expected);
}

TEST_F(BigEndianSerializationTest, SerializeShortProducesCorrectBytes) {
    BigEndianSerialization serializer;
    uint64_t value = 0xABCD;
    std::vector<uint8_t> expected = {0xAB, 0xCD};

    std::vector<uint8_t> result = serializer.serializeShort(value);

    EXPECT_EQ(result, expected);
}

TEST_F(BigEndianSerializationTest, SerializeCharProducesCorrectBytes) {
    BigEndianSerialization serializer;
    uint64_t value = 0x7F;
    std::vector<uint8_t> expected = {0x7F};

    std::vector<uint8_t> result = serializer.serializeChar(value);

    EXPECT_EQ(result, expected);
}

TEST_F(BigEndianSerializationTest, DeserializeIntProducesCorrectValue) {
    BigEndianSerialization serializer;
    std::vector<uint8_t> data = {0x12, 0x34, 0x56, 0x78};
    uint64_t expected = 0x12345678;

    uint64_t result = serializer.deserializeInt(data);

    EXPECT_EQ(result, expected);
}

TEST_F(BigEndianSerializationTest, DeserializeIntWithInsufficientDataReturnsZero) {
    BigEndianSerialization serializer;
    std::vector<uint8_t> data = {0x12, 0x34};

    uint64_t result = serializer.deserializeInt(data);

    EXPECT_EQ(result, 0);
}

TEST_F(BigEndianSerializationTest, DeserializeLongProducesCorrectValue) {
    BigEndianSerialization serializer;
    std::vector<uint8_t> data = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    uint64_t expected = 0x0123456789ABCDEF;

    uint64_t result = serializer.deserializeLong(data);

    EXPECT_EQ(result, expected);
}

TEST_F(BigEndianSerializationTest, DeserializeLongWithInsufficientDataReturnsZero) {
    BigEndianSerialization serializer;
    std::vector<uint8_t> data = {0x01, 0x23, 0x45};

    uint64_t result = serializer.deserializeLong(data);

    EXPECT_EQ(result, 0);
}

TEST_F(BigEndianSerializationTest, DeserializeShortProducesCorrectValue) {
    BigEndianSerialization serializer;
    std::vector<uint8_t> data = {0xAB, 0xCD};
    uint64_t expected = 0xABCD;

    uint64_t result = serializer.deserializeShort(data);

    EXPECT_EQ(result, expected);
}

TEST_F(BigEndianSerializationTest, DeserializeCharProducesCorrectValue) {
    BigEndianSerialization serializer;
    std::vector<uint8_t> data = {0x7F};
    uint64_t expected = 0x7F;

    uint64_t result = serializer.deserializeChar(data);

    EXPECT_EQ(result, expected);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}