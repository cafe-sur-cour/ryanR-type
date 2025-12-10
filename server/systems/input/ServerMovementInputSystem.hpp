/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ServerMovementInputSystem
*/

#ifndef SERVERMOVEMENTINPUTSYSTEM_HPP_
#define SERVERMOVEMENTINPUTSYSTEM_HPP_

#include "../../../common/systems/base/ASystem.hpp"
#include "../../../common/components/temporary/InputIntentComponent.hpp"
#include "../../../common/InputMapping/IInputProvider.hpp"
#include "../../../common/InputMapping/InputAction.hpp"
#include "../../initResourcesManager/ServerInputProvider.hpp"
#include <memory>

namespace ecs {

class ServerMovementInputSystem : public ASystem {
    public:
        ServerMovementInputSystem();
        ~ServerMovementInputSystem() = default;

        void update(std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<Registry> registry, float deltaTime) override;

    private:
        math::Vector2f getMovementDirection(std::shared_ptr<IInputProvider> inputProvider, size_t clientID) const;
        void updateInputIntent(std::shared_ptr<Registry> registry, Entity entityId, const math::Vector2f &direction);
        math::Vector2f getAnalogStickInput(std::shared_ptr<IInputProvider> inputProvider, size_t clientID) const;
        math::Vector2f normalizeDirection(const math::Vector2f &direction) const;
};

} // namespace ecs

#endif /* !SERVERMOVEMENTINPUTSYSTEM_HPP_ */