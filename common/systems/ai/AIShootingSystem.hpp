/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AIShootingSystem - R-Type style AI shooting with cadence
*/

#ifndef AISHOOTINGSYSTEM_HPP
#define AISHOOTINGSYSTEM_HPP

#include "../base/ASystem.hpp"
#include "../../components/permanent/ShootingStatsComponent.hpp"
#include "../../components/permanent/TransformComponent.hpp"

namespace ecs {

class AIShootingSystem : public ASystem {
public:
    void update(
        std::shared_ptr<ResourceManager> resourceManager,
        std::shared_ptr<Registry> registry,
        float deltaTime) override;
};

} // namespace ecs

#endif /* !AISHOOTINGSYSTEM_HPP */

