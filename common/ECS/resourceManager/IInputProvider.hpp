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

namespace ecs {

class IInputProvider {
    public:
        using event_t = gfx::EventType;
        virtual ~IInputProvider() = default;
        virtual bool isKeyPressed(event_t key) = 0;
        virtual float getAxisValue(event_t axis) = 0;
        virtual std::pair<int, int> getMousePos() = 0;
        virtual bool isMouseButtonPressed(int button) = 0;
};

} // namespace ecs

#endif /* !IINPUTPROVIDER_HPP_ */
