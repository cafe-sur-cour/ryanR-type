/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** PowerUpTag
*/

#ifndef POWERUPTAG_HPP_
#define POWERUPTAG_HPP_

namespace ecs {

#include "../base/AComponent.hpp"

class PowerUpTag : public AComponent {
    public:
        PowerUpTag() = default;
        ~PowerUpTag() override = default;

    protected:
    private:
};

}  // namespace ecs

#endif /* !POWERUPTAG_HPP_ */
