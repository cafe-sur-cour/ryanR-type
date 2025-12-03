/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ProjectilePassThroughTag
*/

#ifndef PROJECTILEPASSTHROUGHTAG_HPP_
#define PROJECTILEPASSTHROUGHTAG_HPP_

#include "../base/AComponent.hpp"

namespace ecs {

class ProjectilePassThroughTag : public AComponent {
    public:
        ProjectilePassThroughTag() = default;
        ~ProjectilePassThroughTag() = default;
};

} // namespace ecs

#endif /* !PROJECTILEPASSTHROUGHTAG_HPP_ */