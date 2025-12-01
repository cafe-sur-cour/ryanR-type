/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AnimationRenderingSystem
*/

#ifndef ANIMATIONRENDERINGSYSTEM_HPP_
#define ANIMATIONRENDERINGSYSTEM_HPP_


#include "../../../common/systems/base/ASystem.hpp"
#include <memory>
namespace ecs {

class AnimationRenderingSystem : public ASystem {
    public:
        AnimationRenderingSystem();
        ~AnimationRenderingSystem() override = default;

    protected:
        void update(std::shared_ptr<ResourceManager> resourceManager,
        std::shared_ptr<Registry> registry, float deltaTime) override;
};

}  // namespace ecs

#endif /* !ANIMATIONRENDERINGSYSTEM_HPP_ */