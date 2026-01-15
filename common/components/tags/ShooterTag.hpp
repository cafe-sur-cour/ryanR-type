/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ShooterTag
*/

#ifndef SHOOTERTAG_HPP_
#define SHOOTERTAG_HPP_

#include "../base/IComponent.hpp"

namespace ecs {

class ShooterTag : public IComponent {
    public:
        ShooterTag() = default;
        ~ShooterTag() = default;
};

}

#endif /* !SHOOTERTAG_HPP_ */
