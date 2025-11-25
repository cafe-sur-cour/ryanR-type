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
    Packet packet(1001);
    packet.setType(0x02);
    packet.setLength(256);

    unsigned int clientId = 5555;
    std::vector<uint8_t> header = packet.packHeaderPacket(clientId);

    EXPECT_EQ(header.size(), 24);
    EXPECT_EQ(header[0], 0x93);

    uint32_t extractedClientId = (header[1] << 24) | (header[2] << 16) | (header[3] << 8) | header[4];
    EXPECT_EQ(extractedClientId, clientId);

    uint64_t extractedSequence = 0;
    for (size_t i = 0; i < 8; ++i) {
        extractedSequence = (extractedSequence << 8) | header[5 + i];
    }
    EXPECT_EQ(extractedSequence, 1001);
    EXPECT_EQ(header[13], 0x02);

    uint64_t extractedLength = 0;
    for (size_t i = 0; i < 8; ++i) {
        extractedLength = (extractedLength << 8) | header[14 + i];
    }
    EXPECT_EQ(extractedLength, 256);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}