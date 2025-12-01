/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ProjectileTag
*/

#ifndef PROJECTILETAG_HPP_
#define PROJECTILETAG_HPP_

#include "../base/AComponent.hpp"

namespace ecs {

class ProjectileTag : public AComponent {
    public:
        ProjectileTag() = default;
        ~ProjectileTag() = default;
};

} // namespace ecs

#endif /* !PROJECTILETAG_HPP_ */