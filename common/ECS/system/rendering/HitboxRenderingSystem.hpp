/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** HitboxRenderingSystem
*/

#ifndef HITBOXRENDERINGSYSTEM_HPP_
#define HITBOXRENDERINGSYSTEM_HPP_

#include "../base/ASystem.hpp"
#include <memory>

namespace ecs {

class HitboxRenderingSystem : public ASystem {
    public:
        HitboxRenderingSystem();
        ~HitboxRenderingSystem() override = default;

    protected:
        void update(std::shared_ptr<ResourceManager> resourceManager,
            std::shared_ptr<ARegistry> registry, float deltaTime) override;
};

} // namespace ecs

#endif /* !HITBOXRENDERINGSYSTEM_HPP_ */