/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** IEvent
*/

#ifndef IEVENT_HPP_
#define IEVENT_HPP_

#include <utility>
#include <memory>
#include <string>
#include "EventTypes.hpp"

namespace gfx {

class IEvent {
    public:
        using event_t = EventType;
        virtual ~IEvent() = default;
        virtual void init() = 0;
        virtual event_t pollEvents() = 0;
        virtual std::string getLastTextInput() = 0;
        virtual void cleanup() = 0;
        virtual std::pair<int, int> getMousePos() = 0;
        virtual bool isKeyPressed(event_t key) = 0;
        virtual bool isMouseButtonPressed(int button) = 0;
        virtual float getAxisValue(event_t axis) = 0;

};

typedef IEvent *(*createEvent_t)(void*, void*);

} // namespace gfx

#endif /* !IEVENT_HPP_ */
