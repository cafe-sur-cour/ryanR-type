/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** HealthSystem
*/

#ifndef HEALTHSYSTEM_HPP_
#define HEALTHSYSTEM_HPP_

#include "../base/ASystem.hpp"
#include <memory>

namespace ecs {

class HealthSystem : public ASystem {
    public:
        HealthSystem();
        ~HealthSystem() override = default;

        void update(
            std::shared_ptr<ResourceManager> resourceManager,
            std::shared_ptr<Registry> registry,
            float deltaTime
        ) override;

    private:
        void _handleDamageUpdates(std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<Registry> registry);
        void _handleHealthUpdates(std::shared_ptr<Registry> registry);
};

}

#endif /* !HEALTHSYSTEM_HPP_ */
