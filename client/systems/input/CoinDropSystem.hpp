/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** CoinDropSystem
*/

#ifndef COINDROPSYSTEM_HPP_
#define COINDROPSYSTEM_HPP_

#include "../../../common/systems/base/ASystem.hpp"
#include "../../../common/ECS/resourceManager/ResourceManager.hpp"
#include "../../../common/ECS/entity/registry/Registry.hpp"

namespace ecs {

class CoinDropSystem : public ASystem {
    public:
        CoinDropSystem();
        ~CoinDropSystem() = default;

        void update(
            std::shared_ptr<ResourceManager> resourceManager, 
            std::shared_ptr<Registry> registry, 
            float deltaTime
        ) override;

    private:
        bool _wasPressed;
};

}  // namespace ecs

#endif /* !COINDROPSYSTEM_HPP_ */
