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

namespace ecs {

class ColliderComponent : public AComponent {
    public:
        ColliderComponent(math::FRect hitbox = math::FRect(0.0f, 0.0f, 0.0f, 0.0f))
            : _hitbox(hitbox) {};
        ~ColliderComponent() = default;

        math::FRect getHitbox() const { return _hitbox; };
        void setHitbox(math::FRect hitbox) { _hitbox = hitbox; };

    private:
        math::FRect _hitbox;
};

}  // namespace ecs

#endif /* !COLLIDERCOMPONENT_HPP_ */
