/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** HealthBarRenderingSystem
*/

#ifndef HEALTHBARRENDERINGSYSTEM_HPP_
#define HEALTHBARRENDERINGSYSTEM_HPP_

#include "../../../common/systems/base/ASystem.hpp"
#include <memory>

namespace ecs {

class HealthBarRenderingSystem : public ASystem {
    public:
        HealthBarRenderingSystem();
        ~HealthBarRenderingSystem() override = default;

    protected:
        void update(std::shared_ptr<ResourceManager> resourceManager,
            std::shared_ptr<Registry> registry, float deltaTime) override;
};

}  // namespace ecs

#endif /* !HEALTHBARRENDERINGSYSTEM_HPP_ */
