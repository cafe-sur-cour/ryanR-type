/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** PowerUpTag
*/

#ifndef POWERUPTAG_HPP_
#define POWERUPTAG_HPP_

#include "../base/IComponent.hpp"

namespace ecs {

class PowerUpTag : public IComponent {
    public:
        PowerUpTag() = default;
        ~PowerUpTag() = default;
};

}  // namespace ecs

#endif /* !POWERUPTAG_HPP_ */
