/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** MobTag
*/

#ifndef MOBTAG_HPP_
#define MOBTAG_HPP_

#include "AComponent.hpp"

namespace ecs {

class MobTag : public AComponent {
    public:
        MobTag() = default;
        ~MobTag() = default;
};

} // namespace ecs

#endif /* !MOBTAG_HPP_ */