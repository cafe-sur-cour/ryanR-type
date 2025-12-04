/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** GameZoneColliderTag
*/

#ifndef GAMEZONECOLLIDERTAG_HPP_
#define GAMEZONECOLLIDERTAG_HPP_

#include "../base/AComponent.hpp"

namespace ecs {

class GameZoneColliderTag : public AComponent {
    public:
        GameZoneColliderTag() = default;
        ~GameZoneColliderTag() = default;
};

} // namespace ecs

#endif /* !GAMEZONECOLLIDERTAG_HPP_ */