/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** DeathAnimationSpawner
*/

#ifndef DEATHANIMATIONSPAWNER_HPP_
#define DEATHANIMATIONSPAWNER_HPP_

#include <memory>
#include <string>
#include "../common/resourceManager/ResourceManager.hpp"
#include "../common/ECS/entity/registry/Registry.hpp"
#include "../common/ECS/entity/Entity.hpp"
#include "../common/types/Vector2f.hpp"

class DeathAnimationSpawner {
    public:
        static void spawnDeathAnimation(
            std::shared_ptr<ResourceManager> resourceManager,
            std::shared_ptr<ecs::Registry> registry,
            ecs::Entity entity
        );

    private:
        static math::Vector2f getFirstHitboxCenter(
            std::shared_ptr<ecs::Registry> registry,
            ecs::Entity entity
        );
};

#endif /* !DEATHANIMATIONSPAWNER_HPP_ */