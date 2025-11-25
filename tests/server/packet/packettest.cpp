#include <gtest/gtest.h>
#include "../../../libs/Packet/Packet.hpp"
#include <vector>
#include <cstring>

class PacketTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up test fixtures
    }
    void TearDown() override {
        // Clean up after tests
    }
};


TEST_F(PacketTest, ConstructorInitializesCorrectly) {
    int sequence = 42;
    Packet packet(sequence);

    EXPECT_EQ(packet.getMagicNumber(), 0x93);
    EXPECT_EQ(packet.getSequenceNumber(), 42);
    EXPECT_EQ(packet.getType(), 0x00);
    EXPECT_EQ(packet.getLength(), 0);
}

TEST_F(PacketTest, SettersAndGettersWork) {
    Packet packet(1);

    packet.setType(0x05);
    EXPECT_EQ(packet.getType(), 0x05);

    packet.setLength(128);
    EXPECT_EQ(packet.getLength(), 128);
}

TEST_F(PacketTest, PackHeaderPacketProducesCorrectHeader) {
    Packet packet(1);

    std::vector<uint8_t> header = packet.packHeaderPacket(10, 1, 0x03);

    EXPECT_EQ(header.size(), 16);
    EXPECT_EQ(header[0], 0x93);
    EXPECT_EQ(header[1], 0x00);
    EXPECT_EQ(header[2], 0x00);
    EXPECT_EQ(header[3], 0x00);
    EXPECT_EQ(header[4], 0x0a);
    EXPECT_EQ(header[5], 0x00);
    EXPECT_EQ(header[6], 0x00);
    EXPECT_EQ(header[7], 0x00);
    EXPECT_EQ(header[8], 0x01);
    EXPECT_EQ(header[9], 0x03);
    EXPECT_EQ(header[10], 0x00);
    EXPECT_EQ(header[11], 0x00);
    EXPECT_EQ(header[12], 0x00);
    EXPECT_EQ(header[13], 0x07);
    EXPECT_EQ(header[14], 0x0d);
    EXPECT_EQ(header[15], 0x0a);
}

TEST_F(PacketTest, PackBodyPacketProducesCorrectBody) {
    Packet packet(1);

    std::vector<uint8_t> payload = {0x03, 0x04};
    std::vector<uint8_t> body = packet.packBodyPacket(payload);

    EXPECT_EQ(body.size(), 7);
    EXPECT_EQ(body[0], 0x03);
    EXPECT_EQ(body[1], 0x00);
    EXPECT_EQ(body[2], 0x00);
    EXPECT_EQ(body[3], 0x00);
    EXPECT_EQ(body[4], 0x04);
    EXPECT_EQ(body[5], 0x0d);
    EXPECT_EQ(body[6], 0x0a);
}

TEST_F(PacketTest, PackBodyFailure) {
    Packet packet(1);

    std::vector<uint8_t> payload = {0xFF, 0x04};
    std::vector<uint8_t> body = packet.packBodyPacket(payload);

    EXPECT_TRUE(body.empty());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}