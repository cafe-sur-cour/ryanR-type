/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Packet
*/

#ifndef PACKET_MANAGER_HPP_
#define PACKET_MANAGER_HPP_

#include <memory>
#include <map>
#include <vector>
#include <functional>

#include "IPacketManager.hpp"
#include "../Buffer/IBuffer.hpp"
#include "serializer/ISerializer.hpp"



namespace pm {
class PacketManager : public IPacketManager {
    public:
        PacketManager(uint32_t seqNumber);
        ~PacketManager() override;

        uint32_t getLength() const override;
        uint32_t getSequenceNumber() const override;
        uint8_t getType() const override;
        uint8_t getIdClient() const override;
        std::vector<uint64_t> getPayload() const override;

        void setType(uint8_t type) override;
        void setLength(uint32_t length) override;
        void setSequenceNumber(uint32_t sequenceNumber) override;
        void setPayload(std::vector<uint64_t> payload) override;
        void setIdClient(uint8_t idClient) override;

        std::vector<uint64_t> formatString(const std::string str) override;
        std::vector<uint8_t> pack(uint8_t idClient, uint32_t sequenceNumber, uint8_t type, std::vector<uint64_t> payload) override;
        bool unpack(std::vector<uint8_t> data) override;

        void reset() override;
    private:
        uint8_t _idClient;
        uint32_t _sequenceNumber;
        uint8_t _type;
        uint32_t _length;
        std::vector<uint64_t> _payload;
        std::shared_ptr<ISerializer> _serializer;
        std::map<uint8_t, std::function<std::vector<uint8_t>(std::vector<uint64_t>)>> _packetHandlers;
        std::map<uint8_t, std::function<bool(const std::vector<uint8_t>)>> _packetReceived;
        std::map<uint8_t, uint32_t> _packetLengths;

        std::vector<uint8_t> buildConnectionPacket(std::vector<uint64_t> payload);
        bool parseConnectionPacket(const std::vector<uint8_t> payload);
        std::vector<uint8_t> buildAcceptationPacket(std::vector<uint64_t> payload);
        bool parseAcceptationPacket(const std::vector<uint8_t> payload);
        std::vector<uint8_t> buildDisconnectionPacket(std::vector<uint64_t> payload);
        bool parseDisconnectionPacket(const std::vector<uint8_t> payload);
        std::vector<uint8_t> buildEventPacket(std::vector<uint64_t> payload);
        bool parseEventPacket(const std::vector<uint8_t> payload);
        std::vector<uint8_t> buildMapPacket(std::vector<uint64_t> payload);
        bool parseMapPacket(const std::vector<uint8_t> payload);
        std::vector<uint8_t> buildEndGamePacket(std::vector<uint64_t> payload);
        bool parseEndGamePacket(const std::vector<uint8_t> payload);
        std::vector<uint8_t> buildCanStartPacket(std::vector<uint64_t> payload);
        bool parseCanStartPacket(const std::vector<uint8_t> payload);

        /* Pack Game State */
        std::vector<std::tuple<uint8_t, uint32_t, uint64_t>> _lengthComb;
        std::vector<std::function<std::vector<uint8_t>(std::vector<uint64_t>, std::shared_ptr<unsigned int>)>> _packGSFunction;
        std::vector<uint8_t> packPlayerTag(std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i);
        std::vector<uint8_t> packTransformation(std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i);
        std::vector<uint8_t> packSpeed(std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i);
        std::vector<uint8_t> packHealth(std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i);
        std::vector<uint8_t> packCollider(std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i);
        std::vector<uint8_t> packShootingStats(std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i);
        std::vector<uint8_t> packScore(std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i);
        std::vector<uint8_t> packAIMovementPattern(std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i);
        std::vector<uint8_t> packDamage(std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i);
        std::vector<uint8_t> packLifetime(std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i);
        std::vector<uint8_t> packVelocity(std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i);
        std::vector<uint8_t> packAIMoverTag(std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i);

        /* Unpack Game State */
        std::vector<std::function<unsigned int(const std::vector<uint8_t>, unsigned int)>> _unpackGSFunction;
        unsigned int unpackPlayerTag(std::vector<uint8_t> payload, unsigned int i);
        unsigned int unpackTransform(std::vector<uint8_t> payload, unsigned int i);
        unsigned int unpackSpeed(std::vector<uint8_t> payload, unsigned int i);
        unsigned int unpackHealth(std::vector<uint8_t> payload, unsigned int i);
        unsigned int unpackCollider(std::vector<uint8_t> payload, unsigned int i);
        unsigned int unpackShootingStats(std::vector<uint8_t> payload, unsigned int i);
        unsigned int unpackScore(std::vector<uint8_t> payload, unsigned int i);
        unsigned int unpackAIMovementPattern(std::vector<uint8_t> payload, unsigned int i);
        unsigned int unpackDamage(std::vector<uint8_t> payload, unsigned int i);
        unsigned int unpackLifetime(std::vector<uint8_t> payload, unsigned int i);
        unsigned int unpackVelocity(std::vector<uint8_t> payload, unsigned int i);
        unsigned int unpackAIMoverTag(std::vector<uint8_t> payload, unsigned int i);
};
} // namespace pm

extern "C" {
    void *createPacketInstance(unsigned int id);
    int getType();
}

#endif /* !PACKET_MANAGER_HPP_ */
