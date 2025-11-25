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

namespace gfx {

class IEvent {
    public:
        typedef enum event_e {
            UP,
            DOWN,
            LEFT,
            RIGHT,
            SPACE,
            ENTER,
            ESCAPE,
            CLOSE,
            NEXTGAME,
            NEXTGRAPHIC,
            REFRESH,
            MOUSECLICK,
            MOUSERIGHTCLICK,
            MOUSELEFTCLICK,
            MENU,
            NOTHING,
            TYIPING,
            NEXTDIFFICULTY
        } event_t;
        virtual ~IEvent() = default;
        virtual void init() = 0;
        virtual event_t pollEvents(std::pair<int, int> gridSize) = 0;
        virtual void cleanup() = 0;
        virtual std::pair<int, int> getMousePos() = 0;
        virtual bool isKeyPressed(event_t key) = 0;
        virtual bool isMouseButtonPressed(int button) = 0;
        virtual std::string getTypedText() = 0;

};

} // namespace gfx

#endif /* !IEVENT_HPP_ */
