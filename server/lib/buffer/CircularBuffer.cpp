/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Buffer
*/

#include <algorithm>
#include <cstring>

#include "CircularBuffer.hpp"
#include "../../../common/LoaderType.hpp"

CircularBuffer::CircularBuffer() {
    this->_capacity = 0;
    this->_readPos = 0;
    this->_writePos = 0;
    this->_usedSize = 0;
    this->_buffer = nullptr;
    this->_overflowPolicy = OverflowPolicy::OVERWRITE;
}

CircularBuffer::~CircularBuffer() {
    this->deleteBuffer();
}

void CircularBuffer::createBuffer(size_t size) {
    if (this->_buffer != nullptr) {
        deleteBuffer();
    }
    this->_capacity = size;
    this->_buffer = new char[size];
    this->_readPos = 0;
    this->_writePos = 0;
    this->_usedSize = 0;
}

void CircularBuffer::deleteBuffer() {
    if (this->_buffer != nullptr) {
        delete[] this->_buffer;
        this->_buffer = nullptr;
    }
    this->_capacity = 0;
    this->_readPos = 0;
    this->_writePos = 0;
    this->_usedSize = 0;
}

void CircularBuffer::clear() {
    this->_readPos = 0;
    this->_writePos = 0;
    this->_usedSize = 0;
}

char *CircularBuffer::getBuffer() const {
    return this->_buffer;
}

bool CircularBuffer::writeBuffer(const char *data, size_t size) {
    if (this->_buffer == nullptr || size == 0) {
        return false;
    }

    if (size > this->_capacity) {
        if (this->_overflowPolicy == OverflowPolicy::REJECT) {
            return false;
        }
        data += (size - this->_capacity);
        size = this->_capacity;
    }

    if (this->_overflowPolicy == OverflowPolicy::REJECT && size >
        getAvailableSize()) {
        return false;
    }

    size_t bytesToWrite = size;
    while (bytesToWrite > 0) {
        if (isFull() && _overflowPolicy == OverflowPolicy::OVERWRITE) {
            _advanceReadPos(1);
        }
        size_t contiguousSpace = _capacity - _writePos;
        size_t writeChunk = std::min(bytesToWrite, contiguousSpace);

        if (this->_overflowPolicy == OverflowPolicy::REJECT) {
            writeChunk = std::min(writeChunk, getAvailableSize());
        }
        std::memcpy(this->_buffer + this->_writePos, data +
            (size - bytesToWrite), writeChunk);
        _advanceWritePos(writeChunk);
        bytesToWrite -= writeChunk;
        if (this->_overflowPolicy == OverflowPolicy::REJECT &&
            bytesToWrite > 0) {
            return false;
        }
    }

    return true;
}

size_t CircularBuffer::readBuffer(char *data, size_t size) {
    if (this->_buffer == nullptr || data == nullptr || size == 0
        || isEmpty()) {
        return 0;
    }
    size_t bytesToRead = std::min(size, this->_usedSize);
    size_t bytesRead = 0;
    while (bytesRead < bytesToRead) {
        size_t contiguousData = std::min(
            this->_capacity - this->_readPos,
            bytesToRead - bytesRead);
        std::memcpy(data + bytesRead, this->_buffer + this->_readPos,
            contiguousData);
        _advanceReadPos(contiguousData);
        bytesRead += contiguousData;
    }
    return bytesRead;
}

size_t CircularBuffer::peek(char *data, size_t size, size_t offset) const {
    if (this->_buffer == nullptr || data == nullptr || size == 0 ||
        isEmpty() || offset >= this->_usedSize) {
        return 0;
    }

    size_t bytesToPeek = std::min(size, this->_usedSize - offset);
    size_t peekPos = _getNextPos(this->_readPos, offset);
    size_t bytesPeeked = 0;
    while (bytesPeeked < bytesToPeek) {
        size_t contiguousData = std::min(
            this->_capacity - peekPos,
            bytesToPeek - bytesPeeked);
        std::memcpy(data + bytesPeeked, this->_buffer + peekPos,
            contiguousData);
        peekPos = _getNextPos(peekPos, contiguousData);
        bytesPeeked += contiguousData;
    }

    return bytesPeeked;
}

size_t CircularBuffer::getCapacity() const {
    return this->_capacity;
}

size_t CircularBuffer::getUsedSize() const {
    return _usedSize;
}

size_t CircularBuffer::getAvailableSize() const {
    return this->_capacity - this->_usedSize;
}

bool CircularBuffer::isEmpty() const {
    return _usedSize == 0;
}

bool CircularBuffer::isFull() const {
    return this->_usedSize == this->_capacity;
}

void CircularBuffer::setOverflowPolicy(OverflowPolicy policy) {
    this->_overflowPolicy = policy;
}

OverflowPolicy CircularBuffer::getOverflowPolicy() const {
    return this->_overflowPolicy;
}

size_t CircularBuffer::_getNextPos(size_t pos, size_t offset) const {
    return (pos + offset) % this->_capacity;
}

void CircularBuffer::_advanceReadPos(size_t count) {
    this->_readPos = _getNextPos(this->_readPos, count);
    this->_usedSize -= count;
}

void CircularBuffer::_advanceWritePos(size_t count) {
    this->_writePos = _getNextPos(this->_writePos, count);
    this->_usedSize = std::min(this->_usedSize + count, this->_capacity);
}


extern "C" {
    IBuffer *createBufferInstance() {
        return new CircularBuffer();
    }
    int getType() {
        return BUFFER_MODULE;
    }
}
