/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SpawnIntentComponent
*/

#ifndef SPAWNINTENTCOMPONENT_HPP_
#define SPAWNINTENTCOMPONENT_HPP_

#include "../base/AComponent.hpp"
#include <string>
#include "../../types/Vector2f.hpp"

namespace ecs {

class SpawnIntentComponent : public AComponent {
    public:
        SpawnIntentComponent(const std::string &prefabName, const math::Vector2f &position) :
            _prefabName(prefabName), _position(position) {};
        ~SpawnIntentComponent() = default;

        void setPrefabName(const std::string &prefabName) { _prefabName = prefabName; };
        std::string getPrefabName() const { return _prefabName; };

        void setPosition(const math::Vector2f &position) { _position = position; };
        math::Vector2f getPosition() const { return _position; };

    private:
        std::string _prefabName;
        math::Vector2f _position;
};

}

#endif /* !SPAWNINTENTCOMPONENT_HPP_ */
