/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** TransformComponent
*/

#ifndef TRANSFORMCOMPONENT_HPP_
#define TRANSFORMCOMPONENT_HPP_

#include "AComponent.hpp"
#include "../../types/Vector2f.hpp"

namespace ecs {

class TransformComponent : public AComponent {
    public:
        TransformComponent(Vector2f position = Vector2f(0.0f, 0.0f), float rotation = 0.0f, Vector2f scale = Vector2f(1.0f, 1.0f))
            : _position(position), _rotation(rotation), _scale(scale) {};
        ~TransformComponent() = default;

        Vector2f getPosition() const { return _position; };
        void setPosition(Vector2f position) { _position = position; };

        float getRotation() const { return _rotation; };
        void setRotation(float rotation) { _rotation = rotation; };

        Vector2f getScale() const { return _scale; };
        void setScale(Vector2f scale) { _scale = scale; };

    private:
        Vector2f _position;
        float _rotation;
        Vector2f _scale;
};

} // namespace ecs

#endif /* !TRANSFORMCOMPONENT_HPP_ */
