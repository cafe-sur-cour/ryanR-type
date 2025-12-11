/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** DeathIntentComponent
*/

#ifndef DEATHINTENTCOMPONENT_HPP_
#define DEATHINTENTCOMPONENT_HPP_

#include "../base/AComponent.hpp"
#include "../../ECS/entity/Entity.hpp"

namespace ecs {

class DeathIntentComponent : public AComponent {
    public:
        DeathIntentComponent(ecs::Entity source = 0) : _source(source) {};
        ~DeathIntentComponent() = default;

        ecs::Entity getSource() const { return _source; }
        void setSource(ecs::Entity source) { _source = source; }

    private:
        ecs::Entity _source;
};

}

#endif /* !DEATHINTENTCOMPONENT_HPP_ */
