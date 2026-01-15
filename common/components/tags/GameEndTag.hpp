/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** GameEndTag
*/

#ifndef GAMEENDTAG_HPP_
#define GAMEENDTAG_HPP_

#include "../base/IComponent.hpp"

namespace ecs {

class GameEndTag : public IComponent {
    public:
        GameEndTag() = default;
        ~GameEndTag() = default;
};

}  // namespace ecs

#endif /* !GAMEENDTAG_HPP_ */
