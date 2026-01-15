/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** GameZoneStopTag
*/

#ifndef GAMEZONESTOPTAG_HPP_
#define GAMEZONESTOPTAG_HPP_

#include "../base/IComponent.hpp"

namespace ecs {

class GameZoneStopTag : public IComponent {
    public:
        GameZoneStopTag() = default;
        ~GameZoneStopTag() = default;
};

} // namespace ecs

#endif /* !GAMEZONESTOPTAG_HPP_ */