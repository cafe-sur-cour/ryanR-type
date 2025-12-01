/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SpriteRenderingSystem
*/

#ifndef SPRITERENDERINGSYSTEM_HPP_
#define SPRITERENDERINGSYSTEM_HPP_

#include "../../../common/systems/base/ASystem.hpp"
#include <memory>

namespace ecs {

class SpriteRenderingSystem : public ASystem {
    public:
        SpriteRenderingSystem();
        ~SpriteRenderingSystem() override = default;

    protected:
        void update(std::shared_ptr<ResourceManager> resourceManager,
            std::shared_ptr<Registry> registry, float deltaTime) override;
};

} // namespace ecs

#endif /* !SPRITERENDERINGSYSTEM_HPP_ */
