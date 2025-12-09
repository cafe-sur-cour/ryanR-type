/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** HealthBarComponent
*/

#ifndef HEALTHBARCOMPONENT_HPP_
#define HEALTHBARCOMPONENT_HPP_

#include "../../../common/components/base/AComponent.hpp"

namespace ecs {

class HealthBarComponent : public AComponent {
    public:
        HealthBarComponent() = default;
        ~HealthBarComponent() = default;
};

} // namespace ecs

#endif /* !HEALTHBARCOMPONENT_HPP_ */