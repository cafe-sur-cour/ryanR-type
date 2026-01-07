/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** PowerUpTag
*/

#ifndef POWERUPTAG_HPP_
#define POWERUPTAG_HPP_

#include "../base/AComponent.hpp"

namespace ecs {

class PowerUpTag : public AComponent {
    public:
        PowerUpTag() = default;
        ~PowerUpTag() override = default;

    protected:
    private:
};

}  // namespace ecs

#endif /* !POWERUPTAG_HPP_ */
