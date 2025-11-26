/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** MovementInputSystem
*/

#ifndef MOVEMENTINPUTSYSTEM_HPP_
#define MOVEMENTINPUTSYSTEM_HPP_

#include "../base/ASystem.hpp"
#include "../../component/temporary/MovementIntentComponent.hpp"
#include <unordered_map>

namespace ecs {

enum class MovementKey {
    Left,
    Right,
    Up,
    Down
};

class MovementInputSystem : public ASystem {
    public:
        MovementInputSystem();
        ~MovementInputSystem() = default;

        void update(std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<ARegistry> registry, float deltaTime) override;

        // Simulation methods (will be replaced by real input system)
        void simulateKeyPress(MovementKey key, bool pressed);
        void simulateAxis(float horizontal, float vertical);

    private:
        // Movement key states
        std::unordered_map<MovementKey, bool> _movementKeyStates;
        math::Vector2f _axisInput;

        math::Vector2f getMovementDirection() const;
        void updateMovementIntent(std::shared_ptr<ARegistry> registry, size_t entityId, const math::Vector2f &direction);
};

} // namespace ecs

#endif /* !MOVEMENTINPUTSYSTEM_HPP_ */
