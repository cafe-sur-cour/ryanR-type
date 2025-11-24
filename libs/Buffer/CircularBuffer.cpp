/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Buffer
*/

#include <algorithm>
#include <memory>
#include <vector>
#include <cstring>

#include "CircularBuffer.hpp"
#include "../DLLoader/LoaderType.hpp"

namespace net {

CircularBuffer::CircularBuffer() {
    this->_capacity = 0;
    this->_readPos = 0;
    this->_writePos = 0;
    this->_usedSize = 0;
    this->_overflowPolicy = OverflowPolicy::OVERWRITE;
}

CircularBuffer::~CircularBuffer() {
    this->deleteBuffer();
}

void CircularBuffer::createBuffer(size_t size) {
    this->_buffer.clear();
    this->_capacity = size;
    this->_buffer.resize(size);
    this->_readPos = 0;
    this->_writePos = 0;
    this->_usedSize = 0;
}

void CircularBuffer::deleteBuffer() {
    this->_buffer.clear();
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

std::vector<uint8_t> CircularBuffer::getBuffer() const {
    return this->_buffer;
}

bool CircularBuffer::writeBuffer(
    const std::vector<uint8_t> &data, size_t size) {
    if (this->_buffer.empty())
        return false;
    if (size == 0)
        return true;
    if (data.empty())
        return false;
    if (this->_capacity == 0)
        return false;

    size_t actualSize = std::min(size, data.size());
    if (actualSize > this->_capacity) {
        if (this->_overflowPolicy == OverflowPolicy::REJECT) {
            return false;
        }
        actualSize = this->_capacity;
    }
    if (this->_overflowPolicy == OverflowPolicy::REJECT && actualSize >
        getAvailableSize()) {
        return false;
    }

    size_t bytesToWrite = actualSize;
    size_t dataOffset = (actualSize < size) ? data.size() - actualSize : 0;
    while (bytesToWrite > 0) {
        if (isFull() && _overflowPolicy == OverflowPolicy::OVERWRITE) {
            _advanceReadPos(1);
        }
        size_t contiguousSpace = _capacity - _writePos;
        size_t writeChunk = std::min(bytesToWrite, contiguousSpace);

        if (this->_overflowPolicy == OverflowPolicy::REJECT) {
            writeChunk = std::min(writeChunk, getAvailableSize());
        }

        std::memcpy(this->_buffer.data() + this->_writePos,
                   data.data() + dataOffset + (actualSize - bytesToWrite),
                   writeChunk);
        _advanceWritePos(writeChunk);
        bytesToWrite -= writeChunk;
        if (this->_overflowPolicy == OverflowPolicy::REJECT &&
            bytesToWrite > 0)
            return false;
    }
    return true;
}

std::shared_ptr<std::vector<uint8_t>> CircularBuffer::readBuffer(size_t size) {
    if (this->_buffer.empty() || size == 0 || isEmpty())
        return std::make_unique<std::vector<uint8_t>>();
    size_t bytesToRead = std::min(size, this->_usedSize);
    auto data = std::make_unique<std::vector<uint8_t>>(bytesToRead);
    size_t bytesRead = 0;

    while (bytesRead < bytesToRead) {
        size_t contiguousData = std::min(
            this->_capacity - this->_readPos,
            bytesToRead - bytesRead);
        std::memcpy(data->data() + bytesRead,
                   this->_buffer.data() + this->_readPos,
                   contiguousData);
        _advanceReadPos(contiguousData);
        bytesRead += contiguousData;
    }
    return data;
}

std::shared_ptr<std::vector<uint8_t>> CircularBuffer::peek(
    size_t size, size_t offset) const {
    if (this->_buffer.empty() || size == 0 || isEmpty() ||
        offset >= this->_usedSize)
        return std::make_unique<std::vector<uint8_t>>();
    size_t bytesToPeek = std::min(size, this->_usedSize - offset);
    auto data = std::make_unique<std::vector<uint8_t>>(bytesToPeek);
    size_t peekPos = _getNextPos(this->_readPos, offset);
    size_t bytesPeeked = 0;

    while (bytesPeeked < bytesToPeek) {
        size_t contiguousData = std::min(
            this->_capacity - peekPos,
            bytesToPeek - bytesPeeked);
        std::memcpy(data->data() + bytesPeeked,
                   this->_buffer.data() + peekPos,
                   contiguousData);
        peekPos = _getNextPos(peekPos, contiguousData);
        bytesPeeked += contiguousData;
    }

    return data;
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

}  // namespace net

extern "C" {
    void *createBufferInstance() {
        return new net::CircularBuffer();
    }
    int getType() {
        return BUFFER_MODULE;
    }
}
