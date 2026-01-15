/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ParallaxRenderingSystem
*/

#ifndef PARALLAXRENDERINGSYSTEM_HPP_
#define PARALLAXRENDERINGSYSTEM_HPP_

#include "../../../common/systems/base/ASystem.hpp"
#include "../../components/rendering/ParallaxComponent.hpp"
#include "../../../common/types/Vector2f.hpp"
#include <memory>

namespace ecs {

class ParallaxRenderingSystem : public ASystem {
    public:
        ParallaxRenderingSystem();
        ~ParallaxRenderingSystem() override = default;

    protected:
        void update(std::shared_ptr<ResourceManager> resourceManager,
            std::shared_ptr<Registry> registry, float deltaTime) override;

    private:
        math::Vector2f calculateScale(
            std::shared_ptr<ParallaxLayer> layer,
            float screenWidth,
            float screenHeight
        );

        void renderLayer(
            std::shared_ptr<ParallaxLayer> layer,
            std::shared_ptr<ResourceManager> resourceManager,
            const math::Vector2f& basePosition,
            float screenWidth, float screenHeight);
};

}  // namespace ecs

#endif /* !PARALLAXRENDERINGSYSTEM_HPP_ */
