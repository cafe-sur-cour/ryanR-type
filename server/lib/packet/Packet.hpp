/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Packet
*/

#include <memory>

#include "IPacket.hpp"
#include "../buffer/IBuffer.hpp"
#include "ISerializer.hpp"

#ifndef PACKET_HPP_
#define PACKET_HPP_

class Packet : public IPacket {
    public:
        Packet(int idClient);
        ~Packet() override;

        uint8_t getMagicNumber() const;
        int getIdClient() const;
        size_t getSequenceNumber() const;
        void incrementSequenceNumber();

        bool packPacket(IBuffer &buffer) override;
        bool unpackPacket(IBuffer &buffer) override;

    protected:
    private:
        uint8_t _magicNumber;
        int _idClient;
        size_t _sequenceNumber;
        std::shared_ptr<IBuffer> _buffer;
        std::shared_ptr<ISerializer> _serializer;
};

#endif /* !PACKET_HPP_ */

