/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** GameZoneColliderTag
*/

#ifndef GAMEZONECOLLIDERTAG_HPP_
#define GAMEZONECOLLIDERTAG_HPP_

#include "../base/IComponent.hpp"

namespace ecs {

class GameZoneColliderTag : public IComponent {
    public:
        GameZoneColliderTag() = default;
        ~GameZoneColliderTag() = default;
};

} // namespace ecs

#endif /* !GAMEZONECOLLIDERTAG_HPP_ */