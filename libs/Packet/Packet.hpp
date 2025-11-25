/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Packet
*/

#ifndef PACKET_HPP_
#define PACKET_HPP_

#include <memory>

#include "IPacket.hpp"
#include "../Buffer/IBuffer.hpp"
#include "serializer/ISerializer.hpp"

#define MAGIC_NUMBER 0x93


class Packet : public IPacket {
    public:
        Packet(int idClient);
        ~Packet() override;

        uint8_t getMagicNumber() const;
        int getIdClient() const;
        size_t getSequenceNumber() const;
        void incrementSequenceNumber();

        bool packPacket(const IBuffer &buffer) override;
        bool unpackPacket(const IBuffer &buffer) override;

    protected:
    private:
        uint8_t _magicNumber;
        int _idClient;
        size_t _sequenceNumber;
        std::shared_ptr<IBuffer> _buffer;
        std::shared_ptr<ISerializer> _serializer;
};

extern "C" {
    void *createPacketInstance(int id);
    int getType();
}
#endif /* !PACKET_HPP_ */
