/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Optimized Game state handlers using varint encoding
*/

#include "GameStateHandlersOptimized.hpp"
#include <iostream>
#include <memory>
#include <vector>
#include <utility>
#include <string>
#include "../../libs/Packet/serializer/BigEndianSerialization.hpp"
#include "../../libs/Packet/serializer/Varint.hpp"
#include "../../common/translationToECS.hpp"
#include "../../common/constants.hpp"

using SerializerPtr = std::shared_ptr<pm::ISerializer>;
using pm::BigEndianSerialization;
using pm::Varint;

namespace common::packet {

static SerializerPtr makeSerializer() {
    return std::make_shared<BigEndianSerialization>();
}

static void registerOptimizedGameStatePackers(
    std::shared_ptr<pm::IPacketManager> packet,
    SerializerPtr ser
) {
    auto pushVarint = [](std::vector<uint8_t> &dst, uint64_t val) {
        auto encoded = Varint::encode(val);
        dst.insert(dst.end(), encoded.begin(), encoded.end());
    };

    auto pushUChar = [ser](std::vector<uint8_t> &dst, uint64_t val) {
        auto temp = ser->serializeUChar(val);
        dst.insert(dst.end(), temp.begin(), temp.end());
    };

    /* Transform component - uses varint for positions */
    packet->registerGameStatePackFunction([pushUChar, pushVarint](
        std::vector<uint64_t> payload,
        std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == TRANSFORM) {
            pushUChar(packetData, payload.at(*i));
            pushVarint(packetData, payload.at(*i + 1));
            pushVarint(packetData, payload.at(*i + 2));
            pushVarint(packetData, payload.at(*i + 3));
            pushVarint(packetData, payload.at(*i + 4));
            pushVarint(packetData, payload.at(*i + 5));
            *i += 6;
        }
        return packetData;
    });

    /* Health component */
    packet->registerGameStatePackFunction([pushUChar, pushVarint](
        std::vector<uint64_t> payload,
        std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == HEALTH) {
            pushUChar(packetData, payload.at(*i));
            pushVarint(packetData, payload.at(*i + 1));
            pushVarint(packetData, payload.at(*i + 2));
            *i += 3;
        }
        return packetData;
    });

    /* Score component */
    packet->registerGameStatePackFunction([pushUChar, pushVarint](
        std::vector<uint64_t> payload,
        std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == SCORE) {
            pushUChar(packetData, payload.at(*i));
            pushVarint(packetData, payload.at(*i + 1));
            *i += 2;
        }
        return packetData;
    });

    /* Shot Charge component */
    packet->registerGameStatePackFunction([pushUChar, pushVarint](
        std::vector<uint64_t> payload,
        std::shared_ptr<unsigned int> i
    ) -> std::vector<uint8_t> {
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == CHARGED_SHOT_COMP) {
            pushUChar(packetData, payload.at(*i));
            pushVarint(packetData, payload.at(*i + 1));
            pushVarint(packetData, payload.at(*i + 2));
            pushVarint(packetData, payload.at(*i + 3));
            *i += 4;
        }
        return packetData;
    });

    packet->registerGameStatePackFunction([pushUChar](
        std::vector<uint64_t> payload,
        std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == ANIMATION_STATE) {
            pushUChar(packetData, payload.at(*i));
            *i += 1;
            while (*i + 2 < payload.size()
                && !(payload.at(*i) == static_cast<uint64_t>('\r')
                && payload.at(*i + 1) == static_cast<uint64_t>('\n')
                && payload.at(*i + 2) == static_cast<uint64_t>('\0'))) {
                pushUChar(packetData, payload.at(*i));
                *i += 1;
            }
            pushUChar(packetData, payload.at(*i));
            pushUChar(packetData, payload.at(*i + 1));
            pushUChar(packetData, payload.at(*i + 2));
            *i += 3;
        }
        return packetData;
    });
}

static void registerOptimizedGameStateUnpackers(
    std::shared_ptr<pm::IPacketManager> packet,
    [[maybe_unused]] SerializerPtr ser
) {
    auto readVarintAt =
        [](const std::vector<uint8_t> &payload, unsigned int offset)
            -> std::pair<uint64_t, size_t> {
        return Varint::decode(payload, offset);
    };

    /* Transform component */
    packet->registerGameStateUnpackFunction([readVarintAt, packet](
        const std::vector<uint8_t> payload,
        unsigned int i) -> unsigned int {
        if (payload.at(i) == TRANSFORM) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(TRANSFORM));

            size_t offset = i + 1;
            auto [posX, s1] = readVarintAt(payload,
                static_cast<unsigned int>(offset)); offset += s1;
            auto [posY, s2] = readVarintAt(payload,
                static_cast<unsigned int>(offset)); offset += s2;
            auto [rotation, s3] = readVarintAt(payload,
                static_cast<unsigned int>(offset)); offset += s3;
            auto [scaleX, s4] = readVarintAt(payload,
                static_cast<unsigned int>(offset)); offset += s4;
            auto [scaleY, s5] = readVarintAt(payload,
                static_cast<unsigned int>(offset)); offset += s5;

            vals.push_back(posX);
            vals.push_back(posY);
            vals.push_back(rotation);
            vals.push_back(scaleX);
            vals.push_back(scaleY);
            packet->setPayload(vals);
            return static_cast<unsigned int>(offset - i);
        }
        return 0;
    });

    /* Health component */
    packet->registerGameStateUnpackFunction([readVarintAt, packet](
        const std::vector<uint8_t> payload,
        unsigned int i) -> unsigned int {
        if (payload.at(i) == HEALTH) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(HEALTH));

            size_t offset = i + 1;
            auto [health, s1] = readVarintAt(payload,
                static_cast<unsigned int>(offset)); offset += s1;
            auto [baseHealth, s2] = readVarintAt(payload,
                static_cast<unsigned int>(offset)); offset += s2;

            vals.push_back(health);
            vals.push_back(baseHealth);
            packet->setPayload(vals);
            return static_cast<unsigned int>(offset - i);
        }
        return 0;
    });

    /* Score component */
    packet->registerGameStateUnpackFunction([readVarintAt, packet](
        const std::vector<uint8_t> payload,
        unsigned int i) -> unsigned int {
        if (payload.at(i) == SCORE) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(SCORE));

            auto [score, s1] = readVarintAt(payload, i + 1);
            vals.push_back(score);
            packet->setPayload(vals);
            return static_cast<unsigned int>(1 + s1);
        }
        return 0;
    });

    /* Shot Charge component */
    packet->registerGameStateUnpackFunction([readVarintAt, packet](
        const std::vector<uint8_t> payload,
        unsigned int i
    ) -> unsigned int {
        if (payload.at(i) == CHARGED_SHOT_COMP) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(CHARGED_SHOT_COMP));

            size_t offset = i + 1;

            auto [charge, s1] = readVarintAt(payload, static_cast<unsigned int>(offset));
            offset += s1;
            auto [maxCharge, s2] = readVarintAt(payload, static_cast<unsigned int>(offset));
            offset += s2;
            auto [reloadTime, s3] = readVarintAt(payload, static_cast<unsigned int>(offset));
            offset += s3;

            vals.push_back(charge);
            vals.push_back(maxCharge);
            vals.push_back(reloadTime);
            packet->setPayload(vals);
            return static_cast<unsigned int>(offset - i);
        }
        return 0;
    });

    packet->registerGameStateUnpackFunction([packet](
        const std::vector<uint8_t> payload,
        unsigned int i) -> unsigned int {
        if (payload.at(i) == ANIMATION_STATE) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(ANIMATION_STATE));
            std::string state = "";
            unsigned int j = i + 1;
            while (j < payload.size()) {
                char c = static_cast<char>(payload.at(j));
                if (c == constants::END_OFSTRING_ST) {
                    if (j + 2 < payload.size()
                        && static_cast<char>(payload.at(j + 1)) ==
                            constants::END_OFSTRING_ND
                        && static_cast<char>(payload.at(j + 2)) ==
                            constants::END_OFSTRING_TRD) {
                        j += 3;
                        break;
                    }
                }
                state += c;
                j += 1;
            }
            for (char c : state) {
                vals.push_back(static_cast<uint64_t>(c));
            }
            vals.push_back(static_cast<uint64_t>(constants::END_OFSTRING_ST));
            vals.push_back(static_cast<uint64_t>(constants::END_OFSTRING_ND));
            vals.push_back(static_cast<uint64_t>(constants::END_OFSTRING_TRD));
            packet->setPayload(vals);
            return j - i;
        }
        return 0;
    });
}

bool registerOptimizedGameStateHandlers(std::shared_ptr<pm::IPacketManager> packet) {
    auto ser = makeSerializer();
    registerOptimizedGameStatePackers(packet, ser);
    registerOptimizedGameStateUnpackers(packet, ser);
    return true;
}

bool isOptimizedHandlersAvailable() {
    return true;
}

}  //  namespace common::packet
