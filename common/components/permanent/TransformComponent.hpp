/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** TransformComponent
*/

#ifndef TRANSFORMCOMPONENT_HPP_
#define TRANSFORMCOMPONENT_HPP_

#include "../base/IComponent.hpp"
#include "../../types/Vector2f.hpp"

namespace ecs {

class TransformComponent : public IComponent {
    public:
        TransformComponent(math::Vector2f position = math::Vector2f(0.0f, 0.0f), float rotation = 0.0f, math::Vector2f scale = math::Vector2f(1.0f, 1.0f))
            : _position(position), _rotation(rotation), _scale(scale) {};
        ~TransformComponent() = default;

        math::Vector2f getPosition() const { return _position; };
        void setPosition(math::Vector2f position) { _position = position; };

        float getRotation() const { return _rotation; };
        void setRotation(float rotation) { _rotation = rotation; };

        math::Vector2f getScale() const { return _scale; };
        void setScale(math::Vector2f scale) { _scale = scale; };

    private:
        math::Vector2f _position;
        float _rotation;
        math::Vector2f _scale;
};

} // namespace ecs

#endif /* !TRANSFORMCOMPONENT_HPP_ */
