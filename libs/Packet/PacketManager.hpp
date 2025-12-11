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
        std::vector<uint8_t> buildClientReadyPacket(std::vector<uint64_t> payload);
        bool parseClientReadyPacket(const std::vector<uint8_t> payload);
};
} // namespace pm

extern "C" {
    void *createPacketInstance(unsigned int id);
    int getType();
}

#endif /* !PACKET_MANAGER_HPP_ */
