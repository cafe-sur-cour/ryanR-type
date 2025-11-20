#include <gtest/gtest.h>
#include "../../../server/lib/buffer/CircularBuffer.hpp"
#include <vector>
#include <cstring>
#include <memory>

class CircularBufferTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up test fixtures
    }

    void TearDown() override {
        // Clean up after tests
    }
};

// Test constructor and basic properties
TEST_F(CircularBufferTest, ConstructorInitializesCorrectly) {
    CircularBuffer buffer;
    buffer.createBuffer(1024);
    EXPECT_EQ(buffer.getUsedSize(), 0);
    EXPECT_EQ(buffer.getCapacity(), 1024);
    EXPECT_TRUE(buffer.isEmpty());
    EXPECT_FALSE(buffer.isFull());
}

// Test write operations
TEST_F(CircularBufferTest, WriteIncreasesSize) {
    CircularBuffer buffer;
    buffer.createBuffer(10);
    std::vector<uint8_t> data = {'h', 'e', 'l', 'l', 'o'};

    EXPECT_TRUE(buffer.writeBuffer(data, 5));
    EXPECT_EQ(buffer.getUsedSize(), 5);
    EXPECT_FALSE(buffer.isEmpty());
}

TEST_F(CircularBufferTest, WriteToFullBuffer) {
    CircularBuffer buffer;
    buffer.createBuffer(5);
    std::vector<uint8_t> data = {'h', 'e', 'l', 'l', 'o'};

    EXPECT_TRUE(buffer.writeBuffer(data, 5));
    EXPECT_TRUE(buffer.isFull());

    // Try to write more data to full buffer
    std::vector<uint8_t> moreData = {'w', 'o', 'r', 'l', 'd'};
    EXPECT_TRUE(buffer.writeBuffer(moreData, 5));
}

TEST_F(CircularBufferTest, WritePartialData) {
    CircularBuffer buffer;
    buffer.createBuffer(10);
    std::vector<uint8_t> data = {'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'};
    // Write 5 bytes to buffer with capacity 10
    EXPECT_TRUE(buffer.writeBuffer(data, 5));
    EXPECT_EQ(buffer.getUsedSize(), 5);

    // Write 3 more bytes (total 8, still under capacity)
    EXPECT_TRUE(buffer.writeBuffer(std::vector<uint8_t>(data.begin() + 5, data.begin() + 8), 3));
    EXPECT_EQ(buffer.getUsedSize(), 8);
}

TEST_F(CircularBufferTest, WriteExceedsCapacity) {
    CircularBuffer buffer;
    buffer.createBuffer(5);
    std::vector<uint8_t> data = {'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'};

    // Try to write 11 bytes to buffer with capacity 5
    EXPECT_TRUE(buffer.writeBuffer(data, 11));
    EXPECT_EQ(buffer.getUsedSize(), 5);
}

// Test read operations
TEST_F(CircularBufferTest, ReadFromEmptyBuffer) {
    CircularBuffer buffer;
    buffer.createBuffer(10);

    auto result = buffer.readBuffer(5);
    EXPECT_EQ(result->size(), 0);
}

TEST_F(CircularBufferTest, ReadDecreasesSize) {
    CircularBuffer buffer;
    buffer.createBuffer(10);
    std::vector<uint8_t> data = {'h', 'e', 'l', 'l', 'o'};

    buffer.writeBuffer(data, 5);
    auto result = buffer.readBuffer(3);
    EXPECT_EQ(result->size(), 3);
    EXPECT_EQ(buffer.getUsedSize(), 2);
}

TEST_F(CircularBufferTest, ReadWriteDataIntegrity) {
    CircularBuffer buffer;
    buffer.createBuffer(10);
    std::vector<uint8_t> data = {'h', 'e', 'l', 'l', 'o'};

    buffer.writeBuffer(data, 5);
    auto result = buffer.readBuffer(5);

    EXPECT_EQ(result->size(), 5);
    EXPECT_EQ(*result, data);
    EXPECT_TRUE(buffer.isEmpty());
}

TEST_F(CircularBufferTest, ReadMoreThanAvailable) {
    CircularBuffer buffer;
    buffer.createBuffer(10);
    std::vector<uint8_t> data = {'h', 'i'};
    std::vector<uint8_t> output(10);

    buffer.writeBuffer(data, 2);
    auto result = buffer.readBuffer(5);

    EXPECT_EQ(result->size(), 2);
    EXPECT_EQ(*result, data);
    EXPECT_TRUE(buffer.isEmpty());
}

// Test circular behavior (wrap around)
TEST_F(CircularBufferTest, CircularWrapAround) {
    CircularBuffer buffer;
    buffer.createBuffer(5);

    // Fill buffer
    std::vector<uint8_t> data = {'1', '2', '3', '4', '5'};
    EXPECT_TRUE(buffer.writeBuffer(data, 5));

    // Read 3 bytes
    auto result1 = buffer.readBuffer(3);
    EXPECT_EQ(result1->size(), 3);
    EXPECT_EQ(*result1, std::vector<uint8_t>({'1', '2', '3'}));

    // Write 3 more bytes (should wrap around)
    std::vector<uint8_t> moreData = {'a', 'b', 'c'};
    EXPECT_TRUE(buffer.writeBuffer(moreData, 3));
    EXPECT_EQ(buffer.getUsedSize(), 5);

    // Read all remaining data
    auto result2 = buffer.readBuffer(5);
    EXPECT_EQ(result2->size(), 5);
    EXPECT_EQ(*result2, std::vector<uint8_t>({'4', '5', 'a', 'b', 'c'}));
}

TEST_F(CircularBufferTest, MultipleWrapArounds) {
    CircularBuffer buffer;
    buffer.createBuffer(4);

    for (int i = 0; i < 3; ++i) {
        std::vector<uint8_t> data = {'a', 'b', 'c', 'd'};
        buffer.writeBuffer(data, 4);
        auto result1 = buffer.readBuffer(2);
        EXPECT_EQ(result1->size(), 2);
        EXPECT_EQ(*result1, std::vector<uint8_t>({'a', 'b'}));
        auto result2 = buffer.readBuffer(2);
        EXPECT_EQ(result2->size(), 2);
        EXPECT_EQ(*result2, std::vector<uint8_t>({'c', 'd'}));
        EXPECT_TRUE(buffer.isEmpty());
    }
}

// Test peek operations (if available)
TEST_F(CircularBufferTest, PeekDoesNotModifyBuffer) {
    CircularBuffer buffer;
    buffer.createBuffer(10);
    std::vector<uint8_t> data = {'h', 'e', 'l', 'l', 'o'};
    std::vector<uint8_t> output(10);

    buffer.writeBuffer(data, 5);
    size_t originalSize = buffer.getUsedSize();

    // Peek method exists
    auto peeked = buffer.peek(3);
    EXPECT_EQ(peeked->size(), 3);
    EXPECT_EQ(buffer.getUsedSize(), originalSize); // Size should not change
    EXPECT_EQ(*peeked, std::vector<uint8_t>({'h', 'e', 'l'}));
}

// Test clear operation
TEST_F(CircularBufferTest, ClearEmptiesBuffer) {
    CircularBuffer buffer;
    buffer.createBuffer(10);
    std::vector<uint8_t> data = {'h', 'e', 'l', 'l', 'o'};

    buffer.writeBuffer(data, 5);
    buffer.clear();

    EXPECT_TRUE(buffer.isEmpty());
    EXPECT_EQ(buffer.getUsedSize(), 0);
    EXPECT_EQ(buffer.getCapacity(), 10);
}

// Test edge cases
TEST_F(CircularBufferTest, ZeroCapacityBuffer) {
    CircularBuffer buffer;
    buffer.createBuffer(0);
    std::vector<uint8_t> data = {'t', 'e', 's', 't'};

    EXPECT_FALSE(buffer.writeBuffer(data, 4));
    auto result = buffer.readBuffer(4);
    EXPECT_EQ(result->size(), 0);
    EXPECT_TRUE(buffer.isEmpty());
    EXPECT_TRUE(buffer.isFull());
}

TEST_F(CircularBufferTest, WriteReadZeroBytes) {
    CircularBuffer buffer;
    buffer.createBuffer(10);

    EXPECT_TRUE(buffer.writeBuffer(std::vector<uint8_t>{'t', 'e', 's', 't'}, 0));
    auto result = buffer.readBuffer(0);
    EXPECT_EQ(result->size(), 0);
    EXPECT_EQ(buffer.getUsedSize(), 0);
}

TEST_F(CircularBufferTest, NullPointerHandling) {
    CircularBuffer buffer;
    buffer.createBuffer(10);
    std::vector<uint8_t> emptyData;

    EXPECT_FALSE(buffer.writeBuffer(emptyData, 5));
    auto result = buffer.readBuffer(5);
    EXPECT_EQ(result->size(), 0);
}

// Stress test
TEST_F(CircularBufferTest, StressTestWriteRead) {
    CircularBuffer buffer;
    buffer.createBuffer(100);
    std::vector<uint8_t> testData(50, 'A');

    for (int i = 0; i < 1000; ++i) {
        // Fill with pattern
        std::fill(testData.begin(), testData.end(), 'A' + (i % 26));

        buffer.writeBuffer(testData, 50);
        auto result = buffer.readBuffer(50);

        EXPECT_EQ(result->size(), 50);
        EXPECT_EQ(*result, testData);
    }
}

// Test available space
TEST_F(CircularBufferTest, AvailableSpace) {
    CircularBuffer buffer;
    buffer.createBuffer(10);

    EXPECT_EQ(buffer.getAvailableSize(), 10);

    std::vector<uint8_t> data = {'h', 'e', 'l', 'l', 'o'};
    buffer.writeBuffer(data, 5);
    EXPECT_EQ(buffer.getAvailableSize(), 5);

    auto result = buffer.readBuffer(2);
    EXPECT_EQ(buffer.getAvailableSize(), 7);
}
