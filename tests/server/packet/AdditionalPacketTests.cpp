/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Additional Packet Manager Tests
*/

#include <gtest/gtest.h>
#include "../../../libs/Packet/PacketManager.hpp"
#include "../../../common/packet/DefaultPacketHandlers.hpp"
#include <vector>
#include <cstring>

class AdditionalPacketTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up test fixtures
        packet = std::make_shared<pm::PacketManager>(0);
        common::packet::registerDefaultPacketHandlers(packet);
    }
    void TearDown() override {
        // Clean up after tests
        packet->clearAllHandlers();
    }

    std::shared_ptr<pm::IPacketManager> packet;
};

// Stress tests
TEST_F(AdditionalPacketTest, PackMultiplePacketsInSequence) {

    for (int i = 0; i < 100; i++) {
        std::vector<uint64_t> payload = {static_cast<uint64_t>(i)};
        std::vector<uint8_t> packedData = packet->pack(
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
        std::vector<uint64_t> payload = {static_cast<uint64_t>(i * 2)};
        std::vector<uint8_t> packedData = packet->pack(
            static_cast<uint8_t>(i % 256),
            static_cast<uint32_t>(i * 3),
            DISCONNECTION_PACKET,
            payload
        );

        auto packet2 = std::make_shared<pm::PacketManager>(0);
        common::packet::registerDefaultPacketHandlers(packet2);
        bool result = packet2->unpack(packedData);

        EXPECT_TRUE(result);
        EXPECT_EQ(packet2->getIdClient(), static_cast<uint8_t>(i % 256));
        EXPECT_EQ(packet2->getSequenceNumber(), static_cast<uint32_t>(i * 3));
        EXPECT_EQ(packet2->getType(), DISCONNECTION_PACKET);
    }
}

// Edge case tests
TEST_F(AdditionalPacketTest, PackWithMaxClientId) {
    std::vector<uint64_t> payload = {1};
    std::vector<uint8_t> packedData = packet->pack(255, 100, ACCEPTATION_PACKET, payload);

    EXPECT_GT(packedData.size(), 0);
    EXPECT_EQ(packedData[1], 255);
}

TEST_F(AdditionalPacketTest, PackWithZeroClientId) {
    std::vector<uint64_t> payload = {1};
    std::vector<uint8_t> packedData = packet->pack(0, 100, ACCEPTATION_PACKET, payload);

    EXPECT_GT(packedData.size(), 0);
    EXPECT_EQ(packedData[1], 0);
}

TEST_F(AdditionalPacketTest, PackWithMaxSequenceNumber) {
    std::vector<uint64_t> payload = {1};
    uint32_t maxSeq = 0xFFFFFFFF;
    std::vector<uint8_t> packedData = packet->pack(10, maxSeq, ACCEPTATION_PACKET, payload);

    auto packet2 = std::make_shared<pm::PacketManager>(0);
    common::packet::registerDefaultPacketHandlers(packet2);
    bool result = packet2->unpack(packedData);

    EXPECT_TRUE(result);
    EXPECT_EQ(packet2->getSequenceNumber(), maxSeq);
}

// Payload size tests
TEST_F(AdditionalPacketTest, PackDisconnectionWithDifferentPayloadValues) {

    for (uint8_t i = 0; i < 255; i++) {
        std::vector<uint64_t> payload = {i};
        std::vector<uint8_t> packedData = packet->pack(1, 1, DISCONNECTION_PACKET, payload);

        auto packet2 = std::make_shared<pm::PacketManager>(0);
        common::packet::registerDefaultPacketHandlers(packet2);
        bool result = packet2->unpack(packedData);

        EXPECT_TRUE(result);
        std::vector<uint64_t> retrieved = packet2->getPayload();
        ASSERT_EQ(retrieved.size(), 1);
        EXPECT_EQ(retrieved[0], i);
    }
}

TEST_F(AdditionalPacketTest, PackAcceptationWithDifferentPayloadValues) {

    for (uint8_t i = 0; i < 255; i++) {
        std::vector<uint64_t> payload = {i};
        std::vector<uint8_t> packedData = packet->pack(1, 1, ACCEPTATION_PACKET, payload);

        auto packet2 = std::make_shared<pm::PacketManager>(0);
        common::packet::registerDefaultPacketHandlers(packet2);
        bool result = packet2->unpack(packedData);

        EXPECT_TRUE(result);
        std::vector<uint64_t> retrieved = packet2->getPayload();
        ASSERT_EQ(retrieved.size(), 1);
        EXPECT_EQ(retrieved[0], i);
    }
}

// Format string tests
TEST_F(AdditionalPacketTest, FormatEmptyString) {
    std::vector<uint64_t> formatted = packet->formatString("");

    EXPECT_GE(formatted.size(), 0);
}

TEST_F(AdditionalPacketTest, FormatShortString) {
    std::string shortStr = "Hello";  // 5 chars, within 8-char limit
    std::vector<uint64_t> formatted = packet->formatString(shortStr);

    EXPECT_GT(formatted.size(), 0);
}

TEST_F(AdditionalPacketTest, FormatMaxLengthString) {
    std::string maxStr = "12345678";  // 8 chars, at the limit
    std::vector<uint64_t> formatted = packet->formatString(maxStr);

    EXPECT_GT(formatted.size(), 0);
}

// Sequence and state tests
TEST_F(AdditionalPacketTest, SequenceNumberIncrement) {
    for (uint32_t i = 0; i < 1000; i++) {
        pm::PacketManager testPacket(i);
        EXPECT_EQ(testPacket.getSequenceNumber(), i);
    }
}

TEST_F(AdditionalPacketTest, ResetMultipleTimes) {
    pm::PacketManager testPacket(100);

    for (int i = 0; i < 10; i++) {
        testPacket.setType(0x05);
        testPacket.setLength(256);
        testPacket.setIdClient(42);
        testPacket.setPayload({1, 2, 3, 4});

        testPacket.reset();

        EXPECT_EQ(testPacket.getIdClient(), 0);
        EXPECT_EQ(testPacket.getSequenceNumber(), 0);
        EXPECT_EQ(testPacket.getType(), NO_OP_PACKET);
        EXPECT_EQ(testPacket.getLength(), 0);
        EXPECT_TRUE(testPacket.getPayload().empty());
    }
}

// Connection packet round trip test
TEST_F(AdditionalPacketTest, PackAndUnpackConnectionRoundTrip) {
    std::vector<uint64_t> payload = {};  // connection packet has no payload (LENGTH_CONNECTION_PACKET = 0)
    std::vector<uint8_t> packedData = packet->pack(25, 777, CONNECTION_CLIENT_PACKET, payload);

    auto packet2 = std::make_shared<pm::PacketManager>(0);
    common::packet::registerDefaultPacketHandlers(packet2);
    bool result = packet2->unpack(packedData);

    EXPECT_TRUE(result);
    EXPECT_EQ(packet2->getIdClient(), 25);
    EXPECT_EQ(packet2->getSequenceNumber(), 777);
    EXPECT_EQ(packet2->getType(), CONNECTION_CLIENT_PACKET);

    std::vector<uint64_t> retrieved = packet2->getPayload();
    EXPECT_EQ(retrieved.size(), 0);  // No payload expected
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
