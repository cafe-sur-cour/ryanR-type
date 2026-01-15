/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** TriggerIntentComponent
*/

#ifndef TRIGGERINTENTCOMPONENT_HPP_
#define TRIGGERINTENTCOMPONENT_HPP_

#include "../base/IComponent.hpp"
#include "../../ECS/entity/Entity.hpp"

namespace ecs {

class TriggerIntentComponent : public IComponent {
    public:
        TriggerIntentComponent(Entity self = 0, Entity other = 0) : _self(self), _other(other) {};
        ~TriggerIntentComponent() override = default;

        Entity getSelf() const { return _self; }
        void setSelf(Entity self) { _self = self; }

        Entity getOther() const { return _other; }
        void setOther(Entity other) { _other = other; }

    private:
        Entity _self;
        Entity _other;
};

}  // namespace ecs

#endif /* !TRIGGERINTENTCOMPONENT_HPP_ */
