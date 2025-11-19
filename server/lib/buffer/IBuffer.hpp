/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** IBuffer
*/

#include <stdio.h>
#include <cstdint>

#ifndef IBUFFER_HPP_
#define IBUFFER_HPP_

class IBuffer {
    public:
        virtual ~IBuffer() = default;


        virtual void createBuffer(size_t size) = 0;
        virtual void deleteBuffer() = 0;
        virtual void clear() = 0;

        virtual bool writeBuffer(const uint8_t* data, size_t size) = 0;
        virtual size_t readBuffer(uint8_t* data, size_t size) = 0;

        virtual size_t getCapacity() const = 0;
        virtual size_t getUsedSize() const = 0;
        virtual size_t getAvailableSize() const = 0;
        virtual bool isEmpty() const = 0;
        virtual bool isFull() const = 0;

        virtual uint8_t *getBuffer() const = 0;
    protected:
    private:
};

#endif /* !IBUFFER_HPP_ */
