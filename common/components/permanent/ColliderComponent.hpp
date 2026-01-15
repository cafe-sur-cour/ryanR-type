/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ColliderComponent
*/

#ifndef COLLIDERCOMPONENT_HPP_
#define COLLIDERCOMPONENT_HPP_

#include "../base/IComponent.hpp"
#include "../../types/FRect.hpp"
#include "../../types/Vector2f.hpp"
#include "../../types/OrientedRect.hpp"
#include <cmath>
#include <algorithm>
#include <limits>
#include <vector>

namespace ecs {

enum class CollisionType {
    None = 0,
    Solid = 1,
    Trigger = 2,
    Push = 3
};

class ColliderComponent : public IComponent {
    public:
        ColliderComponent(
            math::Vector2f offset = math::Vector2f(0.0f, 0.0f),
            math::Vector2f size = math::Vector2f(0.0f, 0.0f),
            CollisionType type = CollisionType::Solid
        ) : _offset(offset), _size(size), _type(type) {};
        ~ColliderComponent() = default;

        math::Vector2f getOffset() const;
        void setOffset(math::Vector2f offset);

        math::Vector2f getSize() const;
        void setSize(math::Vector2f size);

        CollisionType getType() const;
        void setType(CollisionType type);

        math::FRect getHitbox(math::Vector2f entityPosition, math::Vector2f scale = math::Vector2f(1.0f, 1.0f)) const;

        math::FRect getScaledHitbox(math::Vector2f entityPosition, math::Vector2f scale) const;

        math::OrientedRect getOrientedHitbox(math::Vector2f entityPosition, math::Vector2f scale, float rotation) const;

        math::FRect getHitbox(math::Vector2f entityPosition, math::Vector2f scale, float rotation) const;

    private:
        math::Vector2f _offset;
        math::Vector2f _size;
        CollisionType _type;
};

}  // namespace ecs

#endif /* !COLLIDERCOMPONENT_HPP_ */
