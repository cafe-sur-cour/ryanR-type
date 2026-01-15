/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** HealthBarComponent
*/

#ifndef HEALTHBARCOMPONENT_HPP_
#define HEALTHBARCOMPONENT_HPP_

#include "../../../common/components/base/IComponent.hpp"

namespace ecs {

class HealthBarComponent : public IComponent {
    public:
        HealthBarComponent() = default;
        ~HealthBarComponent() = default;
};

} // namespace ecs

#endif /* !HEALTHBARCOMPONENT_HPP_ */