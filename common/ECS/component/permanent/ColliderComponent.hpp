/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ColliderComponent
*/

#ifndef COLLIDERCOMPONENT_HPP_
#define COLLIDERCOMPONENT_HPP_

#include "../base/AComponent.hpp"
#include "../../../types/FRect.hpp"
#include "../../../types/Vector2f.hpp"

namespace ecs {

class ColliderComponent : public AComponent {
    public:
        ColliderComponent(math::Vector2f offset = math::Vector2f(0.0f, 0.0f), math::Vector2f size = math::Vector2f(0.0f, 0.0f))
            : _offset(offset), _size(size) {};
        ~ColliderComponent() = default;

        math::Vector2f getOffset() const { return _offset; };
        void setOffset(math::Vector2f offset) { _offset = offset; };

        math::Vector2f getSize() const { return _size; };
        void setSize(math::Vector2f size) { _size = size; };

        math::FRect getHitbox(math::Vector2f entityPosition) const {
            return math::FRect(entityPosition.getX() + _offset.getX(), entityPosition.getY() + _offset.getY(), _size.getX(), _size.getY());
        };

    private:
        math::Vector2f _offset;
        math::Vector2f _size;
};

}  // namespace ecs

#endif /* !COLLIDERCOMPONENT_HPP_ */
