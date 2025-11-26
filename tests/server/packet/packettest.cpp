#include <gtest/gtest.h>
#include "../../../libs/Packet/PacketManager.hpp"
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
    unsigned int sequence = 42;
    PacketManager packet(sequence);

    EXPECT_EQ(packet.getMagicNumber(), 0x93);
    EXPECT_EQ(packet.getSequenceNumber(), 42);
    EXPECT_EQ(packet.getType(), 0x00);
    EXPECT_EQ(packet.getLength(), 0);
    EXPECT_EQ(packet.getIdClient(), 0);
    EXPECT_TRUE(packet.getPayload().empty());
}

TEST_F(PacketTest, ConstructorWithZeroSequence) {
    PacketManager packet(0);

    EXPECT_EQ(packet.getSequenceNumber(), 0);
    EXPECT_EQ(packet.getMagicNumber(), 0x93);
}

TEST_F(PacketTest, ConstructorWithMaxSequence) {
    PacketManager packet(0xFFFFFFFF);

    EXPECT_EQ(packet.getSequenceNumber(), 0xFFFFFFFF);
}

TEST_F(PacketTest, SettersAndGettersWork) {
    PacketManager packet(1);

    packet.setType(0x05);
    EXPECT_EQ(packet.getType(), 0x05);

    packet.setLength(128);
    EXPECT_EQ(packet.getLength(), 128);

    packet.setSequenceNumber(99);
    EXPECT_EQ(packet.getSequenceNumber(), 99);

    packet.setIdClient(42);
    EXPECT_EQ(packet.getIdClient(), 42);
}

TEST_F(PacketTest, SetPayloadWorks) {
    PacketManager packet(1);

    std::vector<uint64_t> payload = {1, 2, 3, 4, 5};
    packet.setPayload(payload);

    std::vector<uint64_t> retrieved = packet.getPayload();
    ASSERT_EQ(retrieved.size(), 5);
    EXPECT_EQ(retrieved[0], 1);
    EXPECT_EQ(retrieved[4], 5);
}

TEST_F(PacketTest, SetEmptyPayload) {
    PacketManager packet(1);

    std::vector<uint64_t> payload = {1, 2, 3};
    packet.setPayload(payload);
    EXPECT_EQ(packet.getPayload().size(), 3);

    packet.setPayload(std::vector<uint64_t>());
    EXPECT_TRUE(packet.getPayload().empty());
}

TEST_F(PacketTest, PackHeaderPacketProducesCorrectHeader) {
    PacketManager packet(1);

    std::vector<uint8_t> header = packet.pack(10, 1, 0x03);

    EXPECT_EQ(header.size(), 13);
    EXPECT_EQ(header[0], 0x93);
    EXPECT_EQ(header[1], 0x0a);
    EXPECT_EQ(header[2], 0x00);
    EXPECT_EQ(header[3], 0x00);
    EXPECT_EQ(header[5], 0x01);
    EXPECT_EQ(header[6], 0x03);
    EXPECT_EQ(header[7], 0x00);
    EXPECT_EQ(header[8], 0x00);
    EXPECT_EQ(header[9], 0x00);
    EXPECT_EQ(header[10], 0x04);
    EXPECT_EQ(header[11], 0x0d);
    EXPECT_EQ(header[12], 0x0a);
}

TEST_F(PacketTest, PackHeaderWithDifferentClientIds) {
    PacketManager packet(1);

    std::vector<uint8_t> header1 = packet.pack(0, 1, 0x01);
    EXPECT_EQ(header1[1], 0x00);

    std::vector<uint8_t> header2 = packet.pack(255, 1, 0x01);
    EXPECT_EQ(header2[1], 0xFF);
}

TEST_F(PacketTest, PackHeaderWithLargeSequenceNumber) {
    PacketManager packet(1);

    std::vector<uint8_t> header = packet.pack(10, 0x12345678, 0x01);

    EXPECT_EQ(header[2], 0x12);
    EXPECT_EQ(header[3], 0x34);
    EXPECT_EQ(header[4], 0x56);
    EXPECT_EQ(header[5], 0x78);
}

TEST_F(PacketTest, PackHeaderForConnectionPacket) {
    PacketManager packet(1);

    std::vector<uint8_t> header = packet.pack(5, 10, CONNECTION_CLIENT_PACKET);

    EXPECT_EQ(header.size(), 13);
    EXPECT_EQ(header[6], CONNECTION_CLIENT_PACKET);
    EXPECT_EQ(header[10], 11);
}

TEST_F(PacketTest, PackHeaderForAcceptationPacket) {
    PacketManager packet(1);

    std::vector<uint8_t> header = packet.pack(5, 10, ACCEPTATION_PACKET);

    EXPECT_EQ(header[6], ACCEPTATION_PACKET);
    EXPECT_EQ(header[10], 4);
}

TEST_F(PacketTest, PackBodyProducesCorrectDisconnectionBody) {
    PacketManager packet(1);

    std::vector<uint64_t> payload = {DISCONNECTION_PACKET, 0x04};
    std::vector<uint8_t> body = packet.pack(payload);

    EXPECT_EQ(body.size(), 4);
    EXPECT_EQ(body[0], DISCONNECTION_PACKET);
    EXPECT_EQ(body[1], 0x04);
    EXPECT_EQ(body[2], 0x0d);
    EXPECT_EQ(body[3], 0x0a);
}

TEST_F(PacketTest, PackBodyWithEmptyPayload) {
    PacketManager packet(1);

    std::vector<uint64_t> payload = {};
    std::vector<uint8_t> body = packet.pack(payload);

    EXPECT_TRUE(body.empty());
}

TEST_F(PacketTest, PackBodyWithUnknownPacketType) {
    PacketManager packet(1);

    std::vector<uint64_t> payload = {0xFF, 0x04};
    std::vector<uint8_t> body = packet.pack(payload);

    EXPECT_TRUE(body.empty());
}

TEST_F(PacketTest, PackBodyForEventPacket) {
    PacketManager packet(1);

    std::vector<uint64_t> payload = {EVENT_PACKET, 0x01, 0x02};
    std::vector<uint8_t> body = packet.pack(payload);

    EXPECT_EQ(body.size(), 5);
    EXPECT_EQ(body[0], EVENT_PACKET);
    EXPECT_EQ(body[1], 0x01);
    EXPECT_EQ(body[2], 0x02);
    EXPECT_EQ(body[3], 0x0d);
    EXPECT_EQ(body[4], 0x0a);
}

TEST_F(PacketTest, PackBodyForAcceptationPacket) {
    PacketManager packet(1);

    std::vector<uint64_t> payload = {ACCEPTATION_PACKET, 0x2A};
    std::vector<uint8_t> body = packet.pack(payload);

    EXPECT_EQ(body.size(), 4);
    EXPECT_EQ(body[0], ACCEPTATION_PACKET);
    EXPECT_EQ(body[1], 0x2A);
}

TEST_F(PacketTest, UnpackValidHeader) {
    PacketManager packet(1);

    std::vector<uint8_t> data = {
        0x93, 0x0a,
        0x00, 0x00, 0x00, 0x01,
        0x02,
        0x00, 0x00, 0x00, 0x04,
        0x0d, 0x0a
    };

    bool result = packet.unpack(data);
    EXPECT_TRUE(result);
    EXPECT_EQ(packet.getIdClient(), 10);
    EXPECT_EQ(packet.getSequenceNumber(), 1);
    EXPECT_EQ(packet.getType(), 0x02);
    EXPECT_EQ(packet.getLength(), 4);
}

TEST_F(PacketTest, UnpackHeaderWithLargeSequenceNumber) {
    PacketManager packet(1);

    std::vector<uint8_t> data = {
        0x93, 0x0a,
        0x12, 0x34, 0x56, 0x78,
        0x02,
        0x00, 0x00, 0x00, 0x04,
        0x0d, 0x0a
    };

    bool result = packet.unpack(data);
    EXPECT_TRUE(result);
    EXPECT_EQ(packet.getSequenceNumber(), 0x12345678);
}

TEST_F(PacketTest, UnpackHeaderWithInvalidSize) {
    PacketManager packet(1);

    std::vector<uint8_t> data = {0x93, 0x0a, 0x00};

    bool result = packet.unpack(data);
    EXPECT_FALSE(result);
}

TEST_F(PacketTest, UnpackHeaderWithInvalidEndOfPacket) {
    PacketManager packet(1);

    std::vector<uint8_t> data = {
        0x93, 0x0a,
        0x00, 0x00, 0x00, 0x01,
        0x02,
        0x00, 0x00, 0x00, 0x04,
        0xFF, 0xFF
    };

    bool result = packet.unpack(data);
    EXPECT_FALSE(result);
}

TEST_F(PacketTest, UnpackEmptyData) {
    PacketManager packet(1);

    std::vector<uint8_t> data = {};

    bool result = packet.unpack(data);
    EXPECT_FALSE(result);
}

TEST_F(PacketTest, UnpackAcceptationBody) {
    PacketManager packet(1);
    packet.setType(ACCEPTATION_PACKET);
    packet.setLength(4);

    std::vector<uint8_t> data = {
        ACCEPTATION_PACKET,
        0x2A,
        0x0d, 0x0a
    };

    bool result = packet.unpack(data);
    EXPECT_TRUE(result);

    std::vector<uint64_t> payload = packet.getPayload();
    ASSERT_EQ(payload.size(), 1);
    EXPECT_EQ(payload[0], 42);
}

TEST_F(PacketTest, UnpackDisconnectionBody) {
    PacketManager packet(1);
    packet.setType(DISCONNECTION_PACKET);
    packet.setLength(4);

    std::vector<uint8_t> data = {
        DISCONNECTION_PACKET,
        0x15,
        0x0d, 0x0a
    };

    bool result = packet.unpack(data);
    EXPECT_TRUE(result);

    std::vector<uint64_t> payload = packet.getPayload();
    ASSERT_EQ(payload.size(), 1);
    EXPECT_EQ(payload[0], 21);
}

TEST_F(PacketTest, UnpackEventBody) {
    PacketManager packet(1);
    packet.setType(EVENT_PACKET);
    packet.setLength(5);

    std::vector<uint8_t> data = {
        EVENT_PACKET,
        0x01,
        0x02,
        0x0d, 0x0a
    };

    bool result = packet.unpack(data);
    EXPECT_TRUE(result);

    std::vector<uint64_t> payload = packet.getPayload();
    ASSERT_EQ(payload.size(), 2);
    EXPECT_EQ(payload[0], 1);
    EXPECT_EQ(payload[1], 2);
}

TEST_F(PacketTest, UnpackBodyWithTypeMismatch) {
    PacketManager packet(1);
    packet.setType(ACCEPTATION_PACKET);
    packet.setLength(4);

    std::vector<uint8_t> data = {
        DISCONNECTION_PACKET,
        0x2A,
        0x0d, 0x0a
    };

    bool result = packet.unpack(data);
    EXPECT_FALSE(result);
}

TEST_F(PacketTest, UnpackBodyWithInvalidLength) {
    PacketManager packet(1);
    packet.setType(ACCEPTATION_PACKET);
    packet.setLength(10);

    std::vector<uint8_t> data = {
        ACCEPTATION_PACKET,
        0x2A,
        0x0d, 0x0a
    };

    bool result = packet.unpack(data);
    EXPECT_FALSE(result);
}

TEST_F(PacketTest, ResetClearsAllFields) {
    PacketManager packet(100);

    packet.setType(0x05);
    packet.setLength(256);
    packet.setIdClient(42);
    packet.setPayload({1, 2, 3, 4});

    packet.reset();

    EXPECT_EQ(packet.getMagicNumber(), 0x93);
    EXPECT_EQ(packet.getIdClient(), 0);
    EXPECT_EQ(packet.getSequenceNumber(), 0);
    EXPECT_EQ(packet.getType(), NO_OP_PACKET);
    EXPECT_EQ(packet.getLength(), 0);
    EXPECT_TRUE(packet.getPayload().empty());
}

TEST_F(PacketTest, PackAndUnpackHeaderRoundTrip) {
    PacketManager packet1(1);
    std::vector<uint8_t> header = packet1.pack(42, 1234, ACCEPTATION_PACKET);

    PacketManager packet2(0);
    bool result = packet2.unpack(header);

    EXPECT_TRUE(result);
    EXPECT_EQ(packet2.getIdClient(), 42);
    EXPECT_EQ(packet2.getSequenceNumber(), 1234);
    EXPECT_EQ(packet2.getType(), ACCEPTATION_PACKET);
    EXPECT_EQ(packet2.getLength(), 4);
}

TEST_F(PacketTest, PackAndUnpackAcceptationBodyRoundTrip) {
    PacketManager packet1(1);
    std::vector<uint64_t> payload = {ACCEPTATION_PACKET, 99};
    std::vector<uint8_t> body = packet1.pack(payload);

    PacketManager packet2(0);
    packet2.setType(ACCEPTATION_PACKET);
    packet2.setLength(4);
    bool result = packet2.unpack(body);

    EXPECT_TRUE(result);
    std::vector<uint64_t> retrieved = packet2.getPayload();
    ASSERT_EQ(retrieved.size(), 1);
    EXPECT_EQ(retrieved[0], 99);
}

TEST_F(PacketTest, PackAndUnpackEventBodyRoundTrip) {
    PacketManager packet1(1);
    std::vector<uint64_t> payload = {EVENT_PACKET, 7, 8};
    std::vector<uint8_t> body = packet1.pack(payload);

    PacketManager packet2(0);
    packet2.setType(EVENT_PACKET);
    packet2.setLength(5);
    bool result = packet2.unpack(body);

    EXPECT_TRUE(result);
    std::vector<uint64_t> retrieved = packet2.getPayload();
    ASSERT_EQ(retrieved.size(), 2);
    EXPECT_EQ(retrieved[0], 7);
    EXPECT_EQ(retrieved[1], 8);
}

TEST_F(PacketTest, MultipleUnpackCallsOverwriteData) {
    PacketManager packet(1);

    std::vector<uint8_t> header1 = {
        0x93, 0x01, 0x00, 0x00, 0x00, 0x01,
        0x02, 0x00, 0x00, 0x00, 0x04, 0x0d, 0x0a
    };
    packet.unpack(header1);
    EXPECT_EQ(packet.getIdClient(), 1);

    std::vector<uint8_t> header2 = {
        0x93, 0x05, 0x00, 0x00, 0x00, 0x02,
        0x03, 0x00, 0x00, 0x00, 0x04, 0x0d, 0x0a
    };
    packet.unpack(header2);
    EXPECT_EQ(packet.getIdClient(), 5);
    EXPECT_EQ(packet.getSequenceNumber(), 2);
}

TEST_F(PacketTest, PackHeaderWithAllPacketTypes) {
    PacketManager packet(1);

    std::vector<uint8_t> types = {
        CONNECTION_CLIENT_PACKET,
        ACCEPTATION_PACKET,
        DISCONNECTION_PACKET,
        EVENT_PACKET
    };

    for (auto type : types) {
        std::vector<uint8_t> header = packet.pack(1, 1, type);
        EXPECT_EQ(header.size(), 13);
        EXPECT_EQ(header[6], type);
    }
}

TEST_F(PacketTest, ResetAfterPackAndUnpack) {
    PacketManager packet(1);

    packet.pack(10, 100, ACCEPTATION_PACKET);
    std::vector<uint64_t> payload = {ACCEPTATION_PACKET, 50};
    packet.pack(payload);

    packet.reset();

    EXPECT_EQ(packet.getSequenceNumber(), 0);
    EXPECT_EQ(packet.getType(), NO_OP_PACKET);
    EXPECT_TRUE(packet.getPayload().empty());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
