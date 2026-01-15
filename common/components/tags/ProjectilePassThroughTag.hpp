/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ProjectilePassThroughTag
*/

#ifndef PROJECTILEPASSTHROUGHTAG_HPP_
#define PROJECTILEPASSTHROUGHTAG_HPP_

#include "../base/IComponent.hpp"

namespace ecs {

class ProjectilePassThroughTag : public IComponent {
    public:
        ProjectilePassThroughTag() = default;
        ~ProjectilePassThroughTag() = default;
};

} // namespace ecs

#endif /* !PROJECTILEPASSTHROUGHTAG_HPP_ */