/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ColliderComponent
*/

#ifndef COLLIDERCOMPONENT_HPP_
#define COLLIDERCOMPONENT_HPP_

#include "../base/AComponent.hpp"
#include "../../types/FRect.hpp"
#include "../../types/Vector2f.hpp"

namespace ecs {

enum class CollisionType {
    None,
    Solid,
    Bounce,
    Trigger,
    Push
};

class ColliderComponent : public AComponent {
    public:
        ColliderComponent(math::Vector2f offset = math::Vector2f(0.0f, 0.0f), math::Vector2f size = math::Vector2f(0.0f, 0.0f), CollisionType type = CollisionType::Solid)
            : _offset(offset), _size(size), _type(type) {};
        ~ColliderComponent() = default;

        math::Vector2f getOffset() const { return _offset; };
        void setOffset(math::Vector2f offset) { _offset = offset; };

        math::Vector2f getSize() const { return _size; };
        void setSize(math::Vector2f size) { _size = size; };

        CollisionType getType() const { return _type; };
        void setType(CollisionType type) { _type = type; };

        math::FRect getHitbox(math::Vector2f entityPosition, math::Vector2f scale = math::Vector2f(1.0f, 1.0f)) const {
            return math::FRect(entityPosition.getX() + _offset.getX(), entityPosition.getY() + _offset.getY(), _size.getX() * scale.getX(), _size.getY() * scale.getY());
        };

        math::FRect getScaledHitbox(math::Vector2f entityPosition, math::Vector2f scale) const {
            return getHitbox(entityPosition, scale);
        };

    private:
        math::Vector2f _offset;
        math::Vector2f _size;
        CollisionType _type;
};

}  // namespace ecs

#endif /* !COLLIDERCOMPONENT_HPP_ */
