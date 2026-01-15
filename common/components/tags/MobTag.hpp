/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** MobTag
*/

#ifndef MOBTAG_HPP_
#define MOBTAG_HPP_

#include "../base/IComponent.hpp"

namespace ecs {

class MobTag : public IComponent {
    public:
        MobTag() = default;
        ~MobTag() = default;
};

} // namespace ecs

#endif /* !MOBTAG_HPP_ */