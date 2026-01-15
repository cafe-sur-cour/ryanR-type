/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Component Serializer - Helper for converting ECS components to/from packets
*/

#ifndef COMPONENT_SERIALIZER_HPP
#define COMPONENT_SERIALIZER_HPP

#include "ComponentDeltaTracker.hpp"
#include <cstring>
#include <string>

namespace rserv {
class ComponentSerializer {
    public:
        static EntitySnapshot createSnapshotFromComponents(uint32_t entityId, const std::vector<uint64_t>& componentData);
};
} // namespace rserv

#endif // COMPONENT_SERIALIZER_HPP
