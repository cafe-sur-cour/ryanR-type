/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** IPrefab
*/

#ifndef IPREFAB_HPP_
#define IPREFAB_HPP_
#include <memory>
#include "../ECS/entity/registry/ARegistry.hpp"
#include "../ECS/entity/Entity.hpp"

class IPrefab {
    public:
        virtual ~IPrefab() = default;
        virtual ecs::Entity instantiate(const std::shared_ptr<ecs::ARegistry> &registry) = 0;
};

#endif /* !IPREFAB_HPP_ */
