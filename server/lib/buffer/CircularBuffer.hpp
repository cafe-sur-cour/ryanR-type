/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Buffer
*/
#include <vector>

#include "IBuffer.hpp"

#ifndef CircularBuffer_HPP_
#define CircularBuffer_HPP_

enum class OverflowPolicy {
    OVERWRITE,
    REJECT,
    EXPAND
};

class CircularBuffer : public IBuffer {
    public:
        CircularBuffer();
        ~CircularBuffer() override;

        void createBuffer(size_t size) override;
        void deleteBuffer() override;
        void clear() override;
        bool writeBuffer(const uint8_t* data, size_t size) override;
        size_t readBuffer(uint8_t* data, size_t size) override;
        size_t getCapacity() const override;
        size_t getUsedSize() const override;
        size_t getAvailableSize() const override;
        bool isEmpty() const override;
        bool isFull() const override;
        uint8_t* getBuffer() const override;


        /* This function allows use to read a section of the buffer without deleting it contrary ti the read buffer */
        size_t peek(uint8_t* data, size_t size, size_t offset = 0) const;
        void setOverflowPolicy(OverflowPolicy policy);
        OverflowPolicy getOverflowPolicy() const;

    private:
        uint8_t *_buffer;
        size_t _capacity;
        size_t _readPos;
        size_t _writePos;
        size_t _usedSize;
        OverflowPolicy _overflowPolicy;

        size_t _getNextPos(size_t pos, size_t offset) const;
        void _advanceReadPos(size_t count);
        void _advanceWritePos(size_t count);
};

extern "C" {
    IBuffer *createBufferInstance();
    int getType();
}

#endif /* !BUFFER_HPP_ */
