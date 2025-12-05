#include <gtest/gtest.h>
#include "../../../libs/Packet/PacketManager.hpp"
#include <vector>
#include <cstring>

class NewPacketTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up test fixtures
    }
    void TearDown() override {
        // Clean up after tests
    }
};

// Constructor tests
TEST_F(NewPacketTest, ConstructorInitializesCorrectly) {
    unsigned int sequence = 42;
    pm::PacketManager packet(sequence);

    EXPECT_EQ(packet.getSequenceNumber(), 42);
    EXPECT_EQ(packet.getType(), 0x00);
    EXPECT_EQ(packet.getLength(), 0);
    EXPECT_EQ(packet.getIdClient(), 0);
    EXPECT_TRUE(packet.getPayload().empty());
}

TEST_F(NewPacketTest, ConstructorWithZeroSequence) {
    pm::PacketManager packet(0);
    EXPECT_EQ(packet.getSequenceNumber(), 0);
}

TEST_F(NewPacketTest, ConstructorWithMaxSequence) {
    pm::PacketManager packet(0xFFFFFFFF);
    EXPECT_EQ(packet.getSequenceNumber(), 0xFFFFFFFF);
}

// Setters and getters tests
TEST_F(NewPacketTest, SettersAndGettersWork) {
    pm::PacketManager packet(1);

    packet.setType(0x05);
    EXPECT_EQ(packet.getType(), 0x05);

    packet.setLength(128);
    EXPECT_EQ(packet.getLength(), 128);

    packet.setSequenceNumber(99);
    EXPECT_EQ(packet.getSequenceNumber(), 99);

    packet.setIdClient(42);
    EXPECT_EQ(packet.getIdClient(), 42);
}

TEST_F(NewPacketTest, SetPayloadWorks) {
    pm::PacketManager packet(1);
    std::vector<uint64_t> payload = {1, 2, 3, 4, 5};
    packet.setPayload(payload);

    std::vector<uint64_t> retrieved = packet.getPayload();
    ASSERT_EQ(retrieved.size(), 5);
    EXPECT_EQ(retrieved[0], 1);
    EXPECT_EQ(retrieved[4], 5);
}

TEST_F(NewPacketTest, SetEmptyPayload) {
    pm::PacketManager packet(1);
    std::vector<uint64_t> payload = {1, 2, 3};
    packet.setPayload(payload);
    EXPECT_EQ(packet.getPayload().size(), 3);

    packet.setPayload(std::vector<uint64_t>());
    EXPECT_TRUE(packet.getPayload().empty());
}

// Pack tests with valid packet types
TEST_F(NewPacketTest, PackAcceptationPacket) {
    pm::PacketManager packet(1);
    std::vector<uint64_t> payload = {42};
    std::vector<uint8_t> packetData = packet.pack(10, 100, ACCEPTATION_PACKET, payload);

    EXPECT_GT(packetData.size(), 0);
    EXPECT_EQ(packetData[0], 0x93);  // Magic number
    EXPECT_EQ(packetData[1], 10);     // Client ID
    EXPECT_EQ(packetData[6], ACCEPTATION_PACKET);  // Packet type
}

TEST_F(NewPacketTest, PackDisconnectionPacket) {
    pm::PacketManager packet(1);
    std::vector<uint64_t> payload = {5};
    std::vector<uint8_t> packetData = packet.pack(15, 200, DISCONNECTION_PACKET, payload);

    EXPECT_GT(packetData.size(), 0);
    EXPECT_EQ(packetData[0], 0x93);  // Magic number
    EXPECT_EQ(packetData[1], 15);    // Client ID
    EXPECT_EQ(packetData[6], DISCONNECTION_PACKET);  // Packet type
}

TEST_F(NewPacketTest, PackEventPacket) {
    pm::PacketManager packet(1);
    std::vector<uint64_t> payload = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    std::vector<uint8_t> packetData = packet.pack(20, 300, EVENT_PACKET, payload);

    EXPECT_GT(packetData.size(), 0);
    EXPECT_EQ(packetData[0], 0x93);  // Magic number
    EXPECT_EQ(packetData[1], 20);    // Client ID
    EXPECT_EQ(packetData[6], EVENT_PACKET);  // Packet type
}

TEST_F(NewPacketTest, PackConnectionPacket) {
    pm::PacketManager packet(1);
    std::vector<uint64_t> payload = {1, 2, 3, 4, 5, 6, 7, 8};  // CONNECTION requires 8 bytes
    std::vector<uint8_t> packetData = packet.pack(5, 50, CONNECTION_CLIENT_PACKET, payload);

    EXPECT_GT(packetData.size(), 0);
    EXPECT_EQ(packetData[0], 0x93);  // Magic number
    EXPECT_EQ(packetData[1], 5);     // Client ID
    EXPECT_EQ(packetData[6], CONNECTION_CLIENT_PACKET);  // Packet type
}

// Pack with invalid packet type
TEST_F(NewPacketTest, PackWithUnknownTypeReturnsEmpty) {
    pm::PacketManager packet(1);
    std::vector<uint64_t> payload = {1};
    std::vector<uint8_t> packetData = packet.pack(1, 1, 0xFF, payload);

    EXPECT_TRUE(packetData.empty());
}

TEST_F(NewPacketTest, PackWithNoOpTypeReturnsEmpty) {
    pm::PacketManager packet(1);
    std::vector<uint64_t> payload = {1};
    std::vector<uint8_t> packetData = packet.pack(1, 1, NO_OP_PACKET, payload);

    EXPECT_TRUE(packetData.empty());
}

// Unpack and pack round-trip tests
TEST_F(NewPacketTest, PackAndUnpackAcceptationRoundTrip) {
    pm::PacketManager packet1(1);
    std::vector<uint64_t> payload = {99};
    std::vector<uint8_t> packedData = packet1.pack(42, 1234, ACCEPTATION_PACKET, payload);

    pm::PacketManager packet2(0);
    bool result = packet2.unpack(packedData);

    EXPECT_TRUE(result);
    EXPECT_EQ(packet2.getIdClient(), 42);
    EXPECT_EQ(packet2.getSequenceNumber(), 1234);
    EXPECT_EQ(packet2.getType(), ACCEPTATION_PACKET);

    std::vector<uint64_t> retrieved = packet2.getPayload();
    ASSERT_EQ(retrieved.size(), 1);
    EXPECT_EQ(retrieved[0], 99);
}

TEST_F(NewPacketTest, PackAndUnpackDisconnectionRoundTrip) {
    pm::PacketManager packet1(1);
    std::vector<uint64_t> payload = {7};
    std::vector<uint8_t> packedData = packet1.pack(10, 500, DISCONNECTION_PACKET, payload);

    pm::PacketManager packet2(0);
    bool result = packet2.unpack(packedData);

    EXPECT_TRUE(result);
    EXPECT_EQ(packet2.getIdClient(), 10);
    EXPECT_EQ(packet2.getSequenceNumber(), 500);
    EXPECT_EQ(packet2.getType(), DISCONNECTION_PACKET);

    std::vector<uint64_t> retrieved = packet2.getPayload();
    ASSERT_EQ(retrieved.size(), 1);
    EXPECT_EQ(retrieved[0], 7);
}

// Reset tests
TEST_F(NewPacketTest, ResetClearsAllFields) {
    pm::PacketManager packet(100);

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

// Unpack invalid data tests
TEST_F(NewPacketTest, UnpackEmptyDataReturnsFalse) {
    pm::PacketManager packet(1);
    std::vector<uint8_t> data = {};
    bool result = packet.unpack(data);
    EXPECT_FALSE(result);
}

TEST_F(NewPacketTest, UnpackInvalidMagicNumberReturnsFalse) {
    pm::PacketManager packet(1);
    std::vector<uint8_t> data = {
        0xFF, 0x0a,  // Invalid magic number
        0x00, 0x00, 0x00, 0x01,
        0x02,
        0x00, 0x00, 0x00, 0x04,
        0x0d, 0x0a
    };
    bool result = packet.unpack(data);
    EXPECT_FALSE(result);
}

TEST_F(NewPacketTest, UnpackTooShortDataReturnsFalse) {
    pm::PacketManager packet(1);
    std::vector<uint8_t> data = {0x93, 0x0a, 0x00};  // Too short
    bool result = packet.unpack(data);
    EXPECT_FALSE(result);
}

// Format string test
TEST_F(NewPacketTest, FormatStringWorks) {
    pm::PacketManager packet(1);
    std::string testStr = "Hello";
    std::vector<uint64_t> formatted = packet.formatString(testStr);

    EXPECT_GT(formatted.size(), 0);
}

// Sequence number test
TEST_F(NewPacketTest, SequenceNumberPreservedInRoundTrip) {
    pm::PacketManager packet1(1);
    std::vector<uint64_t> payload = {1};
    uint32_t testSeq = 0x12345678;
    std::vector<uint8_t> packedData = packet1.pack(1, testSeq, ACCEPTATION_PACKET, payload);

    pm::PacketManager packet2(0);
    bool result = packet2.unpack(packedData);

    EXPECT_TRUE(result);
    EXPECT_EQ(packet2.getSequenceNumber(), testSeq);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
