#include <gtest/gtest.h>
#include "../../../server/lib/buffer/CircularBuffer.hpp"
#include <vector>
#include <cstring>

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
    const char* data = "hello";

    EXPECT_TRUE(buffer.writeBuffer(data, 5));
    EXPECT_EQ(buffer.getUsedSize(), 5);
    EXPECT_FALSE(buffer.isEmpty());
}

TEST_F(CircularBufferTest, WriteToFullBuffer) {
    CircularBuffer buffer;
    buffer.createBuffer(5);
    const char* data = "hello";

    EXPECT_TRUE(buffer.writeBuffer(data, 5));
    EXPECT_TRUE(buffer.isFull());

    // Try to write more data to full buffer
    const char* moreData = "world";
    EXPECT_TRUE(buffer.writeBuffer(moreData, 5));
}

TEST_F(CircularBufferTest, WritePartialData) {
    CircularBuffer buffer;
    buffer.createBuffer(10);
    const char* data = "hello world";
    // Write 5 bytes to buffer with capacity 10
    EXPECT_TRUE(buffer.writeBuffer(data, 5));
    EXPECT_EQ(buffer.getUsedSize(), 5);

    // Write 3 more bytes (total 8, still under capacity)
    EXPECT_TRUE(buffer.writeBuffer(data + 5, 3));
    EXPECT_EQ(buffer.getUsedSize(), 8);
}

TEST_F(CircularBufferTest, WriteExceedsCapacity) {
    CircularBuffer buffer;
    buffer.createBuffer(5);
    const char* data = "hello world";

    // Try to write 11 bytes to buffer with capacity 5
    EXPECT_TRUE(buffer.writeBuffer(data, 11));
    EXPECT_EQ(buffer.getUsedSize(), 5);
}

// Test read operations
TEST_F(CircularBufferTest, ReadFromEmptyBuffer) {
    CircularBuffer buffer;
    buffer.createBuffer(10);
    char output[10];

    EXPECT_EQ(buffer.readBuffer(output, 5), 0);
}

TEST_F(CircularBufferTest, ReadDecreasesSize) {
    CircularBuffer buffer;
    buffer.createBuffer(10);
    const char* data = "hello";
    char output[10];

    buffer.writeBuffer(data, 5);
    EXPECT_EQ(buffer.readBuffer(output, 3), 3);
    EXPECT_EQ(buffer.getUsedSize(), 2);
}

TEST_F(CircularBufferTest, ReadWriteDataIntegrity) {
    CircularBuffer buffer;
    buffer.createBuffer(10);
    const char* data = "hello";
    char output[10];

    buffer.writeBuffer(data, 5);
    size_t bytesRead = buffer.readBuffer(output, 5);

    EXPECT_EQ(bytesRead, 5);
    EXPECT_EQ(memcmp(data, output, 5), 0);
    EXPECT_TRUE(buffer.isEmpty());
}

TEST_F(CircularBufferTest, ReadMoreThanAvailable) {
    CircularBuffer buffer;
    buffer.createBuffer(10);
    const char* data = "hi";
    char output[10];

    buffer.writeBuffer(data, 2);
    size_t bytesRead = buffer.readBuffer(output, 5);

    EXPECT_EQ(bytesRead, 2);
    EXPECT_EQ(memcmp(data, output, 2), 0);
    EXPECT_TRUE(buffer.isEmpty());
}

// Test circular behavior (wrap around)
TEST_F(CircularBufferTest, CircularWrapAround) {
    CircularBuffer buffer;
    buffer.createBuffer(5);
    char output[10];

    // Fill buffer
    EXPECT_TRUE(buffer.writeBuffer("12345", 5));

    // Read 3 bytes
    EXPECT_EQ(buffer.readBuffer(output, 3), 3);
    EXPECT_EQ(memcmp(output, "123", 3), 0);

    // Write 3 more bytes (should wrap around)
    EXPECT_TRUE(buffer.writeBuffer("abc", 3));
    EXPECT_EQ(buffer.getUsedSize(), 5);

    // Read all remaining data
    EXPECT_EQ(buffer.readBuffer(output, 5), 5);
    EXPECT_EQ(memcmp(output, "45abc", 5), 0);
}

TEST_F(CircularBufferTest, MultipleWrapArounds) {
    CircularBuffer buffer;
    buffer.createBuffer(4);
    char output[10];

    for (int i = 0; i < 3; ++i) {
        buffer.writeBuffer("abcd", 4);
        EXPECT_EQ(buffer.readBuffer(output, 2), 2);
        EXPECT_EQ(memcmp(output, "ab", 2), 0);
        EXPECT_EQ(buffer.readBuffer(output, 2), 2);
        EXPECT_EQ(memcmp(output, "cd", 2), 0);
        EXPECT_TRUE(buffer.isEmpty());
    }
}

// Test peek operations (if available)
TEST_F(CircularBufferTest, PeekDoesNotModifyBuffer) {
    CircularBuffer buffer;
    buffer.createBuffer(10);
    const char* data = "hello";
    char output[10];

    buffer.writeBuffer(data, 5);
    size_t originalSize = buffer.getUsedSize();

    // Peek method exists
    size_t peek_result = buffer.peek(output, 3);
    EXPECT_EQ(peek_result, 3);
    EXPECT_EQ(buffer.getUsedSize(), originalSize); // Size should not change
    EXPECT_EQ(memcmp(output, "hel", 3), 0);
}

// Test clear operation
TEST_F(CircularBufferTest, ClearEmptiesBuffer) {
    CircularBuffer buffer;
    buffer.createBuffer(10);
    const char* data = "hello";

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
    const char* data = "test";
    char output[10];

    EXPECT_FALSE(buffer.writeBuffer(data, 4));
    EXPECT_EQ(buffer.readBuffer(output, 4), 0);
    EXPECT_TRUE(buffer.isEmpty());
    EXPECT_TRUE(buffer.isFull());
}

TEST_F(CircularBufferTest, WriteReadZeroBytes) {
    CircularBuffer buffer;
    buffer.createBuffer(10);
    char output[10];

    EXPECT_TRUE(buffer.writeBuffer("test", 0));
    EXPECT_EQ(buffer.readBuffer(output, 0), 0);
    EXPECT_EQ(buffer.getUsedSize(), 0);
}

TEST_F(CircularBufferTest, NullPointerHandling) {
    CircularBuffer buffer;
    buffer.createBuffer(10);

    EXPECT_FALSE(buffer.writeBuffer(nullptr, 5));
    EXPECT_EQ(buffer.readBuffer(nullptr, 5), 0);
}

// Stress test
TEST_F(CircularBufferTest, StressTestWriteRead) {
    CircularBuffer buffer;
    buffer.createBuffer(100);
    std::vector<char> testData(50, 'A');
    std::vector<char> readData(50);

    for (int i = 0; i < 1000; ++i) {
        // Fill with pattern
        std::fill(testData.begin(), testData.end(), 'A' + (i % 26));

        buffer.writeBuffer(testData.data(), 50);
        size_t bytesRead = buffer.readBuffer(readData.data(), 50);

        EXPECT_EQ(bytesRead, 50);
        EXPECT_EQ(testData, readData);
    }
}

// Test available space
TEST_F(CircularBufferTest, AvailableSpace) {
    CircularBuffer buffer;
    buffer.createBuffer(10);

    EXPECT_EQ(buffer.getAvailableSize(), 10);

    buffer.writeBuffer("hello", 5);
    EXPECT_EQ(buffer.getAvailableSize(), 5);

    char output[5];
    buffer.readBuffer(output, 2);
    EXPECT_EQ(buffer.getAvailableSize(), 7);
}
