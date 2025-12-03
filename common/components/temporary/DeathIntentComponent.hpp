/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** DeathIntentComponent
*/

#ifndef DEATHINTENTCOMPONENT_HPP_
#define DEATHINTENTCOMPONENT_HPP_

#include "../base/AComponent.hpp"

namespace ecs {

class DeathIntentComponent : public AComponent {
    public:
        DeathIntentComponent() = default;
        ~DeathIntentComponent() = default;
};

}

#endif /* !DEATHINTENTCOMPONENT_HPP_ */
