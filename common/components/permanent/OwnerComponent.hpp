/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** OwnerComponent
*/

#ifndef OWNERCOMPONENT_HPP_
#define OWNERCOMPONENT_HPP_

#include "../base/IComponent.hpp"
#include "../ECS/entity/Entity.hpp"

namespace ecs {

class OwnerComponent : public IComponent {
    public:
        OwnerComponent(Entity owner = 0) : _owner(owner) {};
        ~OwnerComponent() = default;

        Entity getOwner() const { return _owner; }
        void setOwner(Entity owner) { _owner = owner; }

    private:
        Entity _owner;
};

}  // namespace ecs

#endif /* !OWNERCOMPONENT_HPP_ */