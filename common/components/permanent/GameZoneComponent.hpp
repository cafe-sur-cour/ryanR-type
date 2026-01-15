/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** GameZoneComponent
*/

#ifndef GAMEZONECOMPONENT_HPP_
#define GAMEZONECOMPONENT_HPP_

#include "../base/IComponent.hpp"
#include "../../types/FRect.hpp"
#include "../../constants.hpp"

namespace ecs {

class GameZoneComponent : public IComponent {
    public:
        GameZoneComponent(math::FRect zone = math::FRect(0.0f, 0.0f, constants::MAX_WIDTH, constants::MAX_HEIGHT))
            : _zone(zone) {};
        ~GameZoneComponent() = default;

        math::FRect getZone() const { return _zone; };
        void setZone(math::FRect zone) { _zone = zone; };

    private:
        math::FRect _zone;
};

}  // namespace ecs

#endif /* !GAMEZONECOMPONENT_HPP_ */
