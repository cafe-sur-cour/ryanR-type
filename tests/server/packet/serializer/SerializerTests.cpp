/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Serializer Tests
*/

#include <gtest/gtest.h>
#include "../../../../libs/Packet/serializer/BigEndianSerialization.hpp"
#include "../../../../libs/Packet/serializer/LittleEndianSerialization.hpp"

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
    pm::BigEndianSerialization serializer;
    uint64_t value = 0x12345678;
    std::vector<uint8_t> expected = {0x12, 0x34, 0x56, 0x78};

    std::vector<uint8_t> result = serializer.serializeUInt(value);

    EXPECT_EQ(result, expected);
}

TEST_F(BigEndianSerializationTest, SerializeULongProducesCorrectBytes) {
    pm::BigEndianSerialization serializer;
    uint64_t value = 0x0123456789ABCDEF;
    std::vector<uint8_t> expected = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};

    std::vector<uint8_t> result = serializer.serializeULong(value);

    EXPECT_EQ(result, expected);
}

TEST_F(BigEndianSerializationTest, SerializeUShortProducesCorrectBytes) {
    pm::BigEndianSerialization serializer;
    uint64_t value = 0xABCD;
    std::vector<uint8_t> expected = {0xAB, 0xCD};

    std::vector<uint8_t> result = serializer.serializeUShort(value);

    EXPECT_EQ(result, expected);
}

TEST_F(BigEndianSerializationTest, SerializeUCharProducesCorrectBytes) {
    pm::BigEndianSerialization serializer;
    uint64_t value = 0x7F;
    std::vector<uint8_t> expected = {0x7F};

    std::vector<uint8_t> result = serializer.serializeUChar(value);

    EXPECT_EQ(result, expected);
}

TEST_F(BigEndianSerializationTest, DeserializeUIntProducesCorrectValue) {
    pm::BigEndianSerialization serializer;
    std::vector<uint8_t> data = {0x12, 0x34, 0x56, 0x78};
    uint64_t expected = 0x12345678;

    uint64_t result = serializer.deserializeUInt(data);

    EXPECT_EQ(result, expected);
}

TEST_F(BigEndianSerializationTest, DeserializeUIntWithInsufficientDataReturnsZero) {
    pm::BigEndianSerialization serializer;
    std::vector<uint8_t> data = {0x12, 0x34};

    uint64_t result = serializer.deserializeUInt(data);

    EXPECT_EQ(result, 0);
}

TEST_F(BigEndianSerializationTest, DeserializeULongProducesCorrectValue) {
    pm::BigEndianSerialization serializer;
    std::vector<uint8_t> data = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    uint64_t expected = 0x0123456789ABCDEF;

    uint64_t result = serializer.deserializeULong(data);

    EXPECT_EQ(result, expected);
}

TEST_F(BigEndianSerializationTest, DeserializeULongWithInsufficientDataReturnsZero) {
    pm::BigEndianSerialization serializer;
    std::vector<uint8_t> data = {0x01, 0x23, 0x45};

    uint64_t result = serializer.deserializeULong(data);

    EXPECT_EQ(result, 0);
}

TEST_F(BigEndianSerializationTest, DeserializeUShortProducesCorrectValue) {
    pm::BigEndianSerialization serializer;
    std::vector<uint8_t> data = {0xAB, 0xCD};
    uint64_t expected = 0xABCD;

    uint64_t result = serializer.deserializeUShort(data);

    EXPECT_EQ(result, expected);
}

TEST_F(BigEndianSerializationTest, DeserializeUCharProducesCorrectValue) {
    pm::BigEndianSerialization serializer;
    std::vector<uint8_t> data = {0x7F};
    uint64_t expected = 0x7F;

    uint64_t result = serializer.deserializeUChar(data);

    EXPECT_EQ(result, expected);
}

// ============================================================================
// Little Endian Serialization Tests
// ============================================================================

class LittleEndianSerializationTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up test fixtures
    }
    void TearDown() override {
        // Clean up after tests
    }
};

TEST_F(LittleEndianSerializationTest, SerializeUIntProducesCorrectBytes) {
    pm::LittleEndianSerialization serializer;
    uint64_t value = 0x12345678;
    std::vector<uint8_t> expected = {0x78, 0x56, 0x34, 0x12};

    std::vector<uint8_t> result = serializer.serializeUInt(value);

    EXPECT_EQ(result, expected);
}

TEST_F(LittleEndianSerializationTest, SerializeUIntWithZero) {
    pm::LittleEndianSerialization serializer;
    uint64_t value = 0x00000000;
    std::vector<uint8_t> expected = {0x00, 0x00, 0x00, 0x00};

    std::vector<uint8_t> result = serializer.serializeUInt(value);

    EXPECT_EQ(result, expected);
}

TEST_F(LittleEndianSerializationTest, SerializeUIntWithMax) {
    pm::LittleEndianSerialization serializer;
    uint64_t value = 0xFFFFFFFF;
    std::vector<uint8_t> expected = {0xFF, 0xFF, 0xFF, 0xFF};

    std::vector<uint8_t> result = serializer.serializeUInt(value);

    EXPECT_EQ(result, expected);
}

TEST_F(LittleEndianSerializationTest, SerializeULongProducesCorrectBytes) {
    pm::LittleEndianSerialization serializer;
    uint64_t value = 0x0123456789ABCDEF;
    std::vector<uint8_t> expected = {0xEF, 0xCD, 0xAB, 0x89, 0x67, 0x45, 0x23, 0x01};

    std::vector<uint8_t> result = serializer.serializeULong(value);

    EXPECT_EQ(result, expected);
}

TEST_F(LittleEndianSerializationTest, SerializeULongWithZero) {
    pm::LittleEndianSerialization serializer;
    uint64_t value = 0x0000000000000000;
    std::vector<uint8_t> expected = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    std::vector<uint8_t> result = serializer.serializeULong(value);

    EXPECT_EQ(result, expected);
}

TEST_F(LittleEndianSerializationTest, SerializeULongWithMax) {
    pm::LittleEndianSerialization serializer;
    uint64_t value = 0xFFFFFFFFFFFFFFFF;
    std::vector<uint8_t> expected = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    std::vector<uint8_t> result = serializer.serializeULong(value);

    EXPECT_EQ(result, expected);
}

TEST_F(LittleEndianSerializationTest, SerializeUShortProducesCorrectBytes) {
    pm::LittleEndianSerialization serializer;
    uint64_t value = 0xABCD;
    std::vector<uint8_t> expected = {0xCD, 0xAB};

    std::vector<uint8_t> result = serializer.serializeUShort(value);

    EXPECT_EQ(result, expected);
}

TEST_F(LittleEndianSerializationTest, SerializeUShortWithZero) {
    pm::LittleEndianSerialization serializer;
    uint64_t value = 0x0000;
    std::vector<uint8_t> expected = {0x00, 0x00};

    std::vector<uint8_t> result = serializer.serializeUShort(value);

    EXPECT_EQ(result, expected);
}

TEST_F(LittleEndianSerializationTest, SerializeUShortWithMax) {
    pm::LittleEndianSerialization serializer;
    uint64_t value = 0xFFFF;
    std::vector<uint8_t> expected = {0xFF, 0xFF};

    std::vector<uint8_t> result = serializer.serializeUShort(value);

    EXPECT_EQ(result, expected);
}

TEST_F(LittleEndianSerializationTest, SerializeUCharProducesCorrectBytes) {
    pm::LittleEndianSerialization serializer;
    uint64_t value = 0x7F;
    std::vector<uint8_t> expected = {0x7F};

    std::vector<uint8_t> result = serializer.serializeUChar(value);

    EXPECT_EQ(result, expected);
}

TEST_F(LittleEndianSerializationTest, SerializeUCharWithZero) {
    pm::LittleEndianSerialization serializer;
    uint64_t value = 0x00;
    std::vector<uint8_t> expected = {0x00};

    std::vector<uint8_t> result = serializer.serializeUChar(value);

    EXPECT_EQ(result, expected);
}

TEST_F(LittleEndianSerializationTest, SerializeUCharWithMax) {
    pm::LittleEndianSerialization serializer;
    uint64_t value = 0xFF;
    std::vector<uint8_t> expected = {0xFF};

    std::vector<uint8_t> result = serializer.serializeUChar(value);

    EXPECT_EQ(result, expected);
}

TEST_F(LittleEndianSerializationTest, DeserializeUIntProducesCorrectValue) {
    pm::LittleEndianSerialization serializer;
    std::vector<uint8_t> data = {0x78, 0x56, 0x34, 0x12};
    uint64_t expected = 0x12345678;

    uint64_t result = serializer.deserializeUInt(data);

    EXPECT_EQ(result, expected);
}

TEST_F(LittleEndianSerializationTest, DeserializeUIntWithZero) {
    pm::LittleEndianSerialization serializer;
    std::vector<uint8_t> data = {0x00, 0x00, 0x00, 0x00};
    uint64_t expected = 0x00000000;

    uint64_t result = serializer.deserializeUInt(data);

    EXPECT_EQ(result, expected);
}

TEST_F(LittleEndianSerializationTest, DeserializeUIntWithMax) {
    pm::LittleEndianSerialization serializer;
    std::vector<uint8_t> data = {0xFF, 0xFF, 0xFF, 0xFF};
    uint64_t expected = 0xFFFFFFFF;

    uint64_t result = serializer.deserializeUInt(data);

    EXPECT_EQ(result, expected);
}

TEST_F(LittleEndianSerializationTest, DeserializeUIntWithInsufficientDataReturnsZero) {
    pm::LittleEndianSerialization serializer;
    std::vector<uint8_t> data = {0x12, 0x34};

    uint64_t result = serializer.deserializeUInt(data);

    EXPECT_EQ(result, 0);
}

TEST_F(LittleEndianSerializationTest, DeserializeULongProducesCorrectValue) {
    pm::LittleEndianSerialization serializer;
    std::vector<uint8_t> data = {0xEF, 0xCD, 0xAB, 0x89, 0x67, 0x45, 0x23, 0x01};
    uint64_t expected = 0x0123456789ABCDEF;

    uint64_t result = serializer.deserializeULong(data);

    EXPECT_EQ(result, expected);
}

TEST_F(LittleEndianSerializationTest, DeserializeULongWithZero) {
    pm::LittleEndianSerialization serializer;
    std::vector<uint8_t> data = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint64_t expected = 0x0000000000000000;

    uint64_t result = serializer.deserializeULong(data);

    EXPECT_EQ(result, expected);
}

TEST_F(LittleEndianSerializationTest, DeserializeULongWithMax) {
    pm::LittleEndianSerialization serializer;
    std::vector<uint8_t> data = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint64_t expected = 0xFFFFFFFFFFFFFFFF;

    uint64_t result = serializer.deserializeULong(data);

    EXPECT_EQ(result, expected);
}

TEST_F(LittleEndianSerializationTest, DeserializeULongWithInsufficientDataReturnsZero) {
    pm::LittleEndianSerialization serializer;
    std::vector<uint8_t> data = {0x01, 0x23, 0x45};

    uint64_t result = serializer.deserializeULong(data);

    EXPECT_EQ(result, 0);
}

TEST_F(LittleEndianSerializationTest, DeserializeUShortProducesCorrectValue) {
    pm::LittleEndianSerialization serializer;
    std::vector<uint8_t> data = {0xCD, 0xAB};
    uint64_t expected = 0xABCD;

    uint64_t result = serializer.deserializeUShort(data);

    EXPECT_EQ(result, expected);
}

TEST_F(LittleEndianSerializationTest, DeserializeUShortWithZero) {
    pm::LittleEndianSerialization serializer;
    std::vector<uint8_t> data = {0x00, 0x00};
    uint64_t expected = 0x0000;

    uint64_t result = serializer.deserializeUShort(data);

    EXPECT_EQ(result, expected);
}

TEST_F(LittleEndianSerializationTest, DeserializeUShortWithMax) {
    pm::LittleEndianSerialization serializer;
    std::vector<uint8_t> data = {0xFF, 0xFF};
    uint64_t expected = 0xFFFF;

    uint64_t result = serializer.deserializeUShort(data);

    EXPECT_EQ(result, expected);
}

TEST_F(LittleEndianSerializationTest, DeserializeUShortWithInsufficientDataReturnsZero) {
    pm::LittleEndianSerialization serializer;
    std::vector<uint8_t> data = {0xAB};

    uint64_t result = serializer.deserializeUShort(data);

    EXPECT_EQ(result, 0);
}

TEST_F(LittleEndianSerializationTest, DeserializeUCharProducesCorrectValue) {
    pm::LittleEndianSerialization serializer;
    std::vector<uint8_t> data = {0x7F};
    uint64_t expected = 0x7F;

    uint64_t result = serializer.deserializeUChar(data);

    EXPECT_EQ(result, expected);
}

TEST_F(LittleEndianSerializationTest, DeserializeUCharWithZero) {
    pm::LittleEndianSerialization serializer;
    std::vector<uint8_t> data = {0x00};
    uint64_t expected = 0x00;

    uint64_t result = serializer.deserializeUChar(data);

    EXPECT_EQ(result, expected);
}

TEST_F(LittleEndianSerializationTest, DeserializeUCharWithMax) {
    pm::LittleEndianSerialization serializer;
    std::vector<uint8_t> data = {0xFF};
    uint64_t expected = 0xFF;

    uint64_t result = serializer.deserializeUChar(data);

    EXPECT_EQ(result, expected);
}

TEST_F(LittleEndianSerializationTest, DeserializeUCharWithEmptyDataReturnsZero) {
    pm::LittleEndianSerialization serializer;
    std::vector<uint8_t> data = {};

    uint64_t result = serializer.deserializeUChar(data);

    EXPECT_EQ(result, 0);
}

TEST_F(LittleEndianSerializationTest, SerializeDeserializeUIntRoundTrip) {
    pm::LittleEndianSerialization serializer;
    uint64_t original = 0xDEADBEEF;

    std::vector<uint8_t> serialized = serializer.serializeUInt(original);
    uint64_t result = serializer.deserializeUInt(serialized);

    EXPECT_EQ(result, original);
}

TEST_F(LittleEndianSerializationTest, SerializeDeserializeULongRoundTrip) {
    pm::LittleEndianSerialization serializer;
    uint64_t original = 0xCAFEBABEDEADBEEF;

    std::vector<uint8_t> serialized = serializer.serializeULong(original);
    uint64_t result = serializer.deserializeULong(serialized);

    EXPECT_EQ(result, original);
}

TEST_F(LittleEndianSerializationTest, SerializeDeserializeUShortRoundTrip) {
    pm::LittleEndianSerialization serializer;
    uint64_t original = 0xBEEF;

    std::vector<uint8_t> serialized = serializer.serializeUShort(original);
    uint64_t result = serializer.deserializeUShort(serialized);

    EXPECT_EQ(result, original);
}

TEST_F(LittleEndianSerializationTest, SerializeDeserializeUCharRoundTrip) {
    pm::LittleEndianSerialization serializer;
    uint64_t original = 0xAA;

    std::vector<uint8_t> serialized = serializer.serializeUChar(original);
    uint64_t result = serializer.deserializeUChar(serialized);

    EXPECT_EQ(result, original);
}

TEST_F(LittleEndianSerializationTest, LittleEndianDifferentFromBigEndian) {
    pm::LittleEndianSerialization littleSerializer;
    pm::BigEndianSerialization bigSerializer;
    uint64_t value = 0x12345678;

    std::vector<uint8_t> littleResult = littleSerializer.serializeUInt(value);
    std::vector<uint8_t> bigResult = bigSerializer.serializeUInt(value);

    EXPECT_NE(littleResult, bigResult);
    EXPECT_EQ(littleResult[0], bigResult[3]);
    EXPECT_EQ(littleResult[1], bigResult[2]);
    EXPECT_EQ(littleResult[2], bigResult[1]);
    EXPECT_EQ(littleResult[3], bigResult[0]);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}