/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** MovementInputSystem
*/

#ifndef MOVEMENTINPUTSYSTEM_HPP_
#define MOVEMENTINPUTSYSTEM_HPP_

#include "../base/ASystem.hpp"
#include "../../component/temporary/InputIntentComponent.hpp"
#include <memory>

namespace gfx {
    class IEvent;
}

namespace ecs {

class MovementInputSystem : public ASystem {
    public:
        MovementInputSystem();
        ~MovementInputSystem() = default;

        void update(std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<ARegistry> registry, float deltaTime) override;

    private:
        math::Vector2f getMovementDirection(std::shared_ptr<ResourceManager> resourceManager) const;
        void updateInputIntent(std::shared_ptr<ARegistry> registry, size_t entityId, const math::Vector2f &direction);
        math::Vector2f getAnalogStickInput(std::shared_ptr<gfx::IEvent> eventSystem) const;
};

} // namespace ecs

#endif /* !MOVEMENTINPUTSYSTEM_HPP_ */
