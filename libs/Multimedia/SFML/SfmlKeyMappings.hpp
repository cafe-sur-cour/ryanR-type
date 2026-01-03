/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** KeyMappings
*/

#ifndef KEYMAPPINGS_HPP_
#define KEYMAPPINGS_HPP_

#include <SFML/Graphics.hpp>
#include <map>
#include "../IEvent.hpp"
#include "../EventTypes.hpp"

namespace gfx {

class KeyMappings {
    public:
        static std::map<IEvent::event_t, sf::Keyboard::Key> createKeyboardMapping();
        static std::map<IEvent::event_t, sf::Mouse::Button> createMouseMapping();
        static std::map<IEvent::event_t, unsigned int> createJoystickButtonMapping();
};

} // namespace gfx

#endif /* !KEYMAPPINGS_HPP_ */
