/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** APrefab
*/

#ifndef APREFAB_HPP_
#define APREFAB_HPP_

#include "IPrefab.hpp"
#include "../ECS/entity/registry/Registry.hpp"

class APrefab : public IPrefab {
    public:
        APrefab() = default;
        virtual ~APrefab() = default;
        ecs::Entity instantiate(const std::shared_ptr<ecs::Registry> &registry) override;
};

#endif /* !APREFAB_HPP_ */
