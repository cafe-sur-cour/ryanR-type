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

#include "../../common/interfaces/IPacketManager.hpp"
#include "../../common/interfaces/IBuffer.hpp"
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
        std::vector<std::vector<uint64_t>> getBatchedPayloads() const override;

        void setType(uint8_t type) override;
        void setLength(uint32_t length) override;
        void setSequenceNumber(uint32_t sequenceNumber) override;
        void setPayload(std::vector<uint64_t> payload) override;
        void setIdClient(uint8_t idClient) override;

        std::vector<uint64_t> formatString(const std::string str) override;
        std::vector<uint8_t> pack(uint8_t idClient, uint32_t sequenceNumber, uint8_t type, std::vector<uint64_t> payload) override;
        std::vector<uint8_t> packBatchedGameState(uint8_t idClient, uint32_t sequenceNumber, const std::vector<std::vector<uint64_t>>& entities) override;
        bool unpack(std::vector<uint8_t> data) override;

        void reset() override;

        void registerBuilder(uint8_t type, std::function<std::vector<uint8_t>(std::vector<uint64_t>)> builder) override;
        void registerParser(uint8_t type, std::function<bool(const std::vector<uint8_t>)> parser) override;
        void registerLength(uint8_t type, uint32_t length) override;
        void registerGameStatePackFunction(std::function<std::vector<uint8_t>(std::vector<uint64_t>, std::shared_ptr<unsigned int>)> func) override;
        void registerGameStateUnpackFunction(std::function<unsigned int(const std::vector<uint8_t>, unsigned int)> func) override;
        void registerLengthCombEntry(uint8_t compType, uint32_t compLength, uint64_t compSize) override;
        void clearAllHandlers() override;

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

        /* Pack Game State */
        std::vector<std::tuple<uint8_t, uint32_t, uint64_t>> _lengthComb;
        std::vector<std::function<std::vector<uint8_t>(std::vector<uint64_t>, std::shared_ptr<unsigned int>)>> _packGSFunction;

        /* Unpack Game State */
        std::vector<std::function<unsigned int(const std::vector<uint8_t>, unsigned int)>> _unpackGSFunction;

        /* Batched Game State */
        std::vector<std::vector<uint64_t>> _batchedPayloads;
};
} // namespace pm

extern "C" {
    void *createPacketInstance(unsigned int id);
    int getType();
}

#endif /* !PACKET_MANAGER_HPP_ */
