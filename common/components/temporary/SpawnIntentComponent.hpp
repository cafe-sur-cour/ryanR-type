/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SpawnIntentComponent
*/

#ifndef SPAWNINTENTCOMPONENT_HPP_
#define SPAWNINTENTCOMPONENT_HPP_

#include "../base/IComponent.hpp"
#include <string>
#include <optional>
#include "../../types/Vector2f.hpp"
#include "../../ECS/entity/EntityCreationContext.hpp"

namespace ecs {

class SpawnIntentComponent : public IComponent {
    public:
        SpawnIntentComponent(
            const std::string &prefabName,
            const math::Vector2f &position,
            float gameViewXTrigger = 0.0f
        ) : _prefabName(prefabName),
            _position(position),
            _creationContext(EntityCreationContext::forLocalClient()),
            _gameViewXTrigger(gameViewXTrigger) {}

        SpawnIntentComponent(
            const std::string &prefabName,
            const math::Vector2f &position,
            const EntityCreationContext &context,
            float gameViewXTrigger = 0.0f
        ) : _prefabName(prefabName),
            _position(position),
            _creationContext(context),
            _gameViewXTrigger(gameViewXTrigger) {}

        ~SpawnIntentComponent() = default;

        void setPrefabName(const std::string &prefabName) { _prefabName = prefabName; }
        std::string getPrefabName() const { return _prefabName; }

        void setPosition(const math::Vector2f &position) { _position = position; }
        math::Vector2f getPosition() const { return _position; }

        void setCreationContext(const EntityCreationContext &context) {
            _creationContext = context;
        }
        EntityCreationContext getCreationContext() const { return _creationContext; }

        void setGameViewXTrigger(const float &gameViewXTrigger) { _gameViewXTrigger = gameViewXTrigger; };
        float getGameViewXTrigger() const { return _gameViewXTrigger; };

    private:
        std::string _prefabName;
        math::Vector2f _position;
        EntityCreationContext _creationContext;
        float _gameViewXTrigger;
};

}

#endif /* !SPAWNINTENTCOMPONENT_HPP_ */
