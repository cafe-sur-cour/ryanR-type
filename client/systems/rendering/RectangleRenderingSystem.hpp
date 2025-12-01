/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** RectangleRenderingSystem
*/

#ifndef RECTANGLERENDERINGSYSTEM_HPP_
#define RECTANGLERENDERINGSYSTEM_HPP_

#include "../../../common/systems/base/ASystem.hpp"
#include <memory>

namespace ecs {

class RectangleRenderingSystem : public ASystem {
    public:
        RectangleRenderingSystem();
        ~RectangleRenderingSystem() override = default;

    protected:
        void update(std::shared_ptr<ResourceManager> resourceManager,
            std::shared_ptr<Registry> registry, float deltaTime) override;
};

} // namespace ecs

#endif /* !RECTANGLERENDERINGSYSTEM_HPP_ */