/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SpawnSystem
*/

#ifndef SPAWNSYSTEM_HPP_
#define SPAWNSYSTEM_HPP_

#include "../base/ASystem.hpp"

namespace ecs {

class SpawnSystem : public ASystem {
    public:
        SpawnSystem() = default;
        ~SpawnSystem() = default;

        void update(
            std::shared_ptr<ResourceManager> resourceManager,
            std::shared_ptr<Registry> registry,
            float deltaTime
        ) override;
};

}

#endif /* !SPAWNSYSTEM_HPP_ */
