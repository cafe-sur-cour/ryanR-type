/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** MovementInputSystem
*/

#ifndef MOVEMENTINPUTSYSTEM_HPP_
#define MOVEMENTINPUTSYSTEM_HPP_

#include "../../../common/systems/base/ASystem.hpp"
#include "../../../common/components/temporary/InputIntentComponent.hpp"
#include "../../../common/ECS/resourceManager/IInputProvider.hpp"
#include "../../../common/ECS/resourceManager/InputAction.hpp"
#include <memory>

namespace gfx {
    class IEvent;
}

namespace ecs {

class MovementInputSystem : public ASystem {
    public:
        MovementInputSystem();
        ~MovementInputSystem() = default;

        void update(std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<Registry> registry, float deltaTime) override;

    private:
        math::Vector2f getMovementDirection(std::shared_ptr<ResourceManager> resourceManager) const;
        void updateInputIntent(std::shared_ptr<Registry> registry, Entity entityId, const math::Vector2f &direction);
        math::Vector2f getAnalogStickInput(std::shared_ptr<IInputProvider> inputProvider) const;
        std::pair<int, double> formatDirection(const math::Vector2f &direction) const;
};

} // namespace ecs

#endif /* !MOVEMENTINPUTSYSTEM_HPP_ */
