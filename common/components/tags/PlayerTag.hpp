/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** PlayerTag
*/

#ifndef PLAYERTAG_HPP_
#define PLAYERTAG_HPP_

#include "../base/AComponent.hpp"

namespace ecs {

class PlayerTag : public AComponent {
    public:
        PlayerTag() = default;
        ~PlayerTag() = default;
};

} // namespace ecs

#endif /* !PLAYERTAG_HPP_ */