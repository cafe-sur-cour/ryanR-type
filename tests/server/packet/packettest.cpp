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
    int clientId = 42;
    Packet packet(clientId);

    EXPECT_EQ(packet.getMagicNumber(), 0x93);
    EXPECT_EQ(packet.getIdClient(), clientId);
    EXPECT_EQ(packet.getSequenceNumber(), 0);
}

TEST_F(PacketTest, IncrementSequenceNumber) {
    Packet packet(1);

    EXPECT_EQ(packet.getSequenceNumber(), 0);
    packet.incrementSequenceNumber();
    EXPECT_EQ(packet.getSequenceNumber(), 1);
    packet.incrementSequenceNumber();
    EXPECT_EQ(packet.getSequenceNumber(), 2);
}

TEST_F(PacketTest, PacketTest_GetSequenceN_Test) {
    Packet packet(5);

    for (size_t i = 0; i < 10; ++i) {
        EXPECT_EQ(packet.getSequenceNumber(), i);
        packet.incrementSequenceNumber();
    }
}
