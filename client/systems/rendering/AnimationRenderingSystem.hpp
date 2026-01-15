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
#include "../../components/rendering/AnimationComponent.hpp"
#include "../../../common/ECS/entity/Entity.hpp"
#include "../../../common/ECS/entity/registry/Registry.hpp"
#include "../../../common/Parser/Animation/AnimationConditionFactory.hpp"
namespace ecs {

class AnimationRenderingSystem : public ASystem {
    public:
        AnimationRenderingSystem();
        ~AnimationRenderingSystem() override = default;

    protected:
        void update(std::shared_ptr<ResourceManager> resourceManager,
        std::shared_ptr<Registry> registry, float deltaTime) override;

    private:
        std::unordered_map<Entity, float> _waitTimers;
};

}  // namespace ecs

#endif /* !ANIMATIONRENDERINGSYSTEM_HPP_ */
