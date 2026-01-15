/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Component Serializer
*/

#include <vector>
#include <utility>
#include <string>
#include "ComponentSerializer.hpp"
#include "../constants.hpp"

rserv::EntitySnapshot rserv::ComponentSerializer::createSnapshotFromComponents(
    uint32_t entityId, const std::vector<uint64_t>& componentData) {
    EntitySnapshot snapshot;
    snapshot.entityId = entityId;
    snapshot.componentMask = 0;

    size_t i = 0;
    while (i < componentData.size()) {
        uint8_t compType = static_cast<uint8_t>(componentData[i]);
        if (compType == SCORE) {
            if (i + 1 < componentData.size()) {
                snapshot.componentMask |= (1u << compType);
                snapshot.components[compType] = {componentData[i + 1]};
                i += 2;
            } else {
                i++;
            }
            continue;
        }

        if (compType == TRANSFORM) {
            if (i + 5 < componentData.size()) {
                snapshot.componentMask |= (1u << compType);
                snapshot.components[compType] = {
                    componentData[i + 1], componentData[i + 2],
                    componentData[i + 3], componentData[i + 4],
                    componentData[i + 5]
                };
                i += 6;
            } else {
                i++;
            }
            continue;
        }

        if (compType == HEALTH) {
            if (i + 2 < componentData.size()) {
                snapshot.componentMask |= (1u << compType);
                snapshot.components[compType] = {
                    componentData[i + 1], componentData[i + 2]
                };
                i += 3;
            } else {
                i++;
            }
            continue;
        }

        if (compType == CHARGED_SHOT_COMP) {
            if (i + 3 < componentData.size()) {
                snapshot.componentMask |= (1u << compType);
                snapshot.components[compType] = {
                    componentData[i + 1], componentData[i + 2],
                    componentData[i + 3]
                };
                i += 4;
            } else {
                i++;
            }
            continue;
        }

        if (compType == ANIMATION_STATE) {
            if (i + 1 < componentData.size()) {
                std::vector<uint64_t> prefabData;
                prefabData.reserve(constants::BITMASK_INT);
                size_t j = i + 1;
                while (j < componentData.size()) {
                    prefabData.push_back(componentData[j]);
                    if (j + 2 < componentData.size() &&
                        componentData[j] == static_cast<uint64_t>(
                            constants::END_OFSTRING_ST) &&
                        componentData[j + 1] == static_cast<uint64_t>(
                            constants::END_OFSTRING_ND) &&
                        componentData[j + 2] == static_cast<uint64_t>(
                            constants::END_OFSTRING_TRD)) {
                        j += 3;
                        break;
                    }
                    j++;
                }
                snapshot.componentMask |= (1u << compType);
                snapshot.components[compType] = std::move(prefabData);
                i = j;
            } else {
                i++;
            }
            continue;
        }
        i++;
    }

    return snapshot;
}
