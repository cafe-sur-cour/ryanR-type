/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** APrefab
*/

#ifndef APREFAB_HPP_
#define APREFAB_HPP_

#include "IPrefab.hpp"
#include "../ECS/entity/registry/ARegistry.hpp"

class APrefab : public IPrefab {
    public:
        APrefab() = default;
        virtual ~APrefab() = default;
        size_t instantiate(const std::shared_ptr<ecs::ARegistry> &registry) override;
};

#endif /* !APREFAB_HPP_ */
