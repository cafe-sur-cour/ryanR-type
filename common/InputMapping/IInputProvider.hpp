/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** IInputProvider
*/

#ifndef IINPUTPROVIDER_HPP_
#define IINPUTPROVIDER_HPP_

#include <utility>
#include "../../libs/Multimedia/EventTypes.hpp"
#include "InputAction.hpp"

namespace ecs {

class IInputProvider {
    public:
        using event_t = gfx::EventType;
        virtual ~IInputProvider() = default;

        virtual float getAxisValue(event_t axis) = 0;
        virtual bool isActionPressed(InputAction action) = 0;
        virtual float getActionAxis(InputAction action) = 0;
};

}  // namespace ecs

#endif /* !IINPUTPROVIDER_HPP_ */
