/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** PlayerProjectileTag
*/

#ifndef PLAYERPROJECTILETAG_HPP_
#define PLAYERPROJECTILETAG_HPP_

#include "../base/AComponent.hpp"

namespace ecs {

class PlayerProjectileTag : public AComponent {
    public:
        PlayerProjectileTag() = default;
        ~PlayerProjectileTag() = default;
};

} // namespace ecs

#endif /* !PLAYERPROJECTILETAG_HPP_ */