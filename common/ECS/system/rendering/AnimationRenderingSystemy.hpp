/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AnimationRenderingSystemy
*/

#ifndef ANIMATIONRENDERINGSYSTEMY_HPP_
#define ANIMATIONRENDERINGSYSTEMY_HPP_


#include "../base/ASystem.hpp"
#include <memory>
namespace ecs {

class AnimationRenderingSystemy : public ASystem {
    public:
        AnimationRenderingSystemy();
        ~AnimationRenderingSystemy() override = default;

    protected:
        void update(std::shared_ptr<ResourceManager> resourceManager,
        std::shared_ptr<ARegistry> registry, float deltaTime) override;
};

}  // namespace ecs

#endif /* !ANIMATIONRENDERINGSYSTEMY_HPP_ */
