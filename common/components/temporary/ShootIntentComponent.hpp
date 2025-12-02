/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ShootIntentComponent
*/

#ifndef SHOOTINTENTCOMPONENT_HPP_
#define SHOOTINTENTCOMPONENT_HPP_

#include "../base/AComponent.hpp"
#include "../../types/Vector2f.hpp"

namespace ecs {

class ShootIntentComponent : public AComponent {
    public:
        ShootIntentComponent() = default;
        ~ShootIntentComponent() = default;

    private:
        math::Vector2f _position;
};

}  // namespace ecs

#endif /* !SHOOTINTENTCOMPONENT_HPP_ */
