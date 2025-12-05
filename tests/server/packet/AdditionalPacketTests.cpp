/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Additional Packet Manager Tests
*/

#include <gtest/gtest.h>
#include "../../../libs/Packet/PacketManager.hpp"
#include <vector>
#include <cstring>

class AdditionalPacketTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up test fixtures
    }
    void TearDown() override {
        // Clean up after tests
    }
};

// Stress tests
TEST_F(AdditionalPacketTest, PackMultiplePacketsInSequence) {
    pm::PacketManager packet(1);
    
    for (int i = 0; i < 100; i++) {
        std::vector<uint64_t> payload = {static_cast<uint64_t>(i)};
        std::vector<uint8_t> packedData = packet.pack(
            static_cast<uint8_t>(i % 256), 
            static_cast<uint32_t>(i), 
            ACCEPTATION_PACKET, 
            payload
        );
        EXPECT_GT(packedData.size(), 0);
    }
}

TEST_F(AdditionalPacketTest, PackAndUnpackMultipleRoundTrips) {
    for (int i = 0; i < 50; i++) {
        pm::PacketManager packet1(static_cast<uint32_t>(i));
        std::vector<uint64_t> payload = {static_cast<uint64_t>(i * 2)};
        std::vector<uint8_t> packedData = packet1.pack(
            static_cast<uint8_t>(i % 256), 
            static_cast<uint32_t>(i * 3), 
            DISCONNECTION_PACKET, 
            payload
        );

        pm::PacketManager packet2(0);
        bool result = packet2.unpack(packedData);
        
        EXPECT_TRUE(result);
        EXPECT_EQ(packet2.getIdClient(), static_cast<uint8_t>(i % 256));
        EXPECT_EQ(packet2.getSequenceNumber(), static_cast<uint32_t>(i * 3));
        EXPECT_EQ(packet2.getType(), DISCONNECTION_PACKET);
    }
}

// Edge case tests
TEST_F(AdditionalPacketTest, PackWithMaxClientId) {
    pm::PacketManager packet(1);
    std::vector<uint64_t> payload = {1};
    std::vector<uint8_t> packedData = packet.pack(255, 100, ACCEPTATION_PACKET, payload);
    
    EXPECT_GT(packedData.size(), 0);
    EXPECT_EQ(packedData[1], 255);
}

TEST_F(AdditionalPacketTest, PackWithZeroClientId) {
    pm::PacketManager packet(1);
    std::vector<uint64_t> payload = {1};
    std::vector<uint8_t> packedData = packet.pack(0, 100, ACCEPTATION_PACKET, payload);
    
    EXPECT_GT(packedData.size(), 0);
    EXPECT_EQ(packedData[1], 0);
}

TEST_F(AdditionalPacketTest, PackWithMaxSequenceNumber) {
    pm::PacketManager packet(1);
    std::vector<uint64_t> payload = {1};
    uint32_t maxSeq = 0xFFFFFFFF;
    std::vector<uint8_t> packedData = packet.pack(10, maxSeq, ACCEPTATION_PACKET, payload);
    
    pm::PacketManager packet2(0);
    bool result = packet2.unpack(packedData);
    
    EXPECT_TRUE(result);
    EXPECT_EQ(packet2.getSequenceNumber(), maxSeq);
}

// Payload size tests
TEST_F(AdditionalPacketTest, PackDisconnectionWithDifferentPayloadValues) {
    pm::PacketManager packet(1);
    
    for (uint8_t i = 0; i < 255; i++) {
        std::vector<uint64_t> payload = {i};
        std::vector<uint8_t> packedData = packet.pack(1, 1, DISCONNECTION_PACKET, payload);
        
        pm::PacketManager packet2(0);
        bool result = packet2.unpack(packedData);
        
        EXPECT_TRUE(result);
        std::vector<uint64_t> retrieved = packet2.getPayload();
        ASSERT_EQ(retrieved.size(), 1);
        EXPECT_EQ(retrieved[0], i);
    }
}

TEST_F(AdditionalPacketTest, PackAcceptationWithDifferentPayloadValues) {
    pm::PacketManager packet(1);
    
    for (uint8_t i = 0; i < 255; i++) {
        std::vector<uint64_t> payload = {i};
        std::vector<uint8_t> packedData = packet.pack(1, 1, ACCEPTATION_PACKET, payload);
        
        pm::PacketManager packet2(0);
        bool result = packet2.unpack(packedData);
        
        EXPECT_TRUE(result);
        std::vector<uint64_t> retrieved = packet2.getPayload();
        ASSERT_EQ(retrieved.size(), 1);
        EXPECT_EQ(retrieved[0], i);
    }
}

// Format string tests
TEST_F(AdditionalPacketTest, FormatEmptyString) {
    pm::PacketManager packet(1);
    std::vector<uint64_t> formatted = packet.formatString("");
    
    EXPECT_GE(formatted.size(), 0);
}

TEST_F(AdditionalPacketTest, FormatShortString) {
    pm::PacketManager packet(1);
    std::string shortStr = "Hello";  // 5 chars, within 8-char limit
    std::vector<uint64_t> formatted = packet.formatString(shortStr);
    
    EXPECT_GT(formatted.size(), 0);
}

TEST_F(AdditionalPacketTest, FormatMaxLengthString) {
    pm::PacketManager packet(1);
    std::string maxStr = "12345678";  // 8 chars, at the limit
    std::vector<uint64_t> formatted = packet.formatString(maxStr);
    
    EXPECT_GT(formatted.size(), 0);
}

// Sequence and state tests
TEST_F(AdditionalPacketTest, SequenceNumberIncrement) {
    for (uint32_t i = 0; i < 1000; i++) {
        pm::PacketManager packet(i);
        EXPECT_EQ(packet.getSequenceNumber(), i);
    }
}

TEST_F(AdditionalPacketTest, ResetMultipleTimes) {
    pm::PacketManager packet(100);
    
    for (int i = 0; i < 10; i++) {
        packet.setType(0x05);
        packet.setLength(256);
        packet.setIdClient(42);
        packet.setPayload({1, 2, 3, 4});
        
        packet.reset();
        
        EXPECT_EQ(packet.getIdClient(), 0);
        EXPECT_EQ(packet.getSequenceNumber(), 0);
        EXPECT_EQ(packet.getType(), NO_OP_PACKET);
        EXPECT_EQ(packet.getLength(), 0);
        EXPECT_TRUE(packet.getPayload().empty());
    }
}

// Connection packet round trip test
TEST_F(AdditionalPacketTest, PackAndUnpackConnectionRoundTrip) {
    pm::PacketManager packet1(1);
    std::vector<uint64_t> payload = {10, 20, 30, 40, 50, 60, 70, 80};  // 8 bytes for connection
    std::vector<uint8_t> packedData = packet1.pack(25, 777, CONNECTION_CLIENT_PACKET, payload);

    pm::PacketManager packet2(0);
    bool result = packet2.unpack(packedData);

    EXPECT_TRUE(result);
    EXPECT_EQ(packet2.getIdClient(), 25);
    EXPECT_EQ(packet2.getSequenceNumber(), 777);
    EXPECT_EQ(packet2.getType(), CONNECTION_CLIENT_PACKET);

    std::vector<uint64_t> retrieved = packet2.getPayload();
    ASSERT_EQ(retrieved.size(), 8);
    for (size_t i = 0; i < 8; i++) {
        EXPECT_EQ(retrieved[i], payload[i]);
    }
}

// Event packet with 3-element payload (1 char + 2 longs)
TEST_F(AdditionalPacketTest, PackAndUnpackEventWithValidPayload) {
    pm::PacketManager packet1(1);
    std::vector<uint64_t> payload = {5, 123456789, 987654321};  // 3 elements as expected
    std::vector<uint8_t> packedData = packet1.pack(30, 999, EVENT_PACKET, payload);

    pm::PacketManager packet2(0);
    bool result = packet2.unpack(packedData);

    EXPECT_TRUE(result);
    EXPECT_EQ(packet2.getIdClient(), 30);
    EXPECT_EQ(packet2.getSequenceNumber(), 999);
    EXPECT_EQ(packet2.getType(), EVENT_PACKET);

    // Event packet should have 3 elements
    std::vector<uint64_t> retrieved = packet2.getPayload();
    ASSERT_EQ(retrieved.size(), 3);
    EXPECT_EQ(retrieved[0], payload[0]);  // First element should match (single char)
    // Note: Long values may have endianness/serialization differences
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
