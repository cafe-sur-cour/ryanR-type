/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AIMovementSystem
*/

#ifndef AIMOVEMENTSYSTEM_HPP
#define AIMOVEMENTSYSTEM_HPP

#include "../base/ASystem.hpp"
#include "../../components/permanent/AIMovementPatternComponent.hpp"
#include "../../components/permanent/TransformComponent.hpp"
#include "../../components/permanent/SpeedComponent.hpp"
#include "../../components/temporary/InputIntentComponent.hpp"
#include <optional>

namespace ecs {

class AIMovementSystem : public ASystem {
public:
    void update(
        std::shared_ptr<ResourceManager> resourceManager,
        std::shared_ptr<Registry> registry,
        float deltaTime) override;

private:
    void executeStraightLine(
        std::shared_ptr<AIMovementPatternComponent> movement,
        std::shared_ptr<TransformComponent> transform,
        std::shared_ptr<InputIntentComponent> inputIntent);
    void executeZigzag(
        std::shared_ptr<AIMovementPatternComponent> movement,
        std::shared_ptr<TransformComponent> transform,
        std::shared_ptr<InputIntentComponent> inputIntent,
        float deltaTime);
    void executeVerticalMirror(
        std::shared_ptr<Registry> registry,
        std::shared_ptr<AIMovementPatternComponent> movement,
        std::shared_ptr<TransformComponent> transform,
        std::shared_ptr<InputIntentComponent> inputIntent);
    std::optional<size_t> findNearestPlayer(
        std::shared_ptr<Registry> registry,
        const math::Vector2f& position,
        float range);
};

} // namespace ecs

#endif /* !AIMOVEMENTSYSTEM_HPP */
