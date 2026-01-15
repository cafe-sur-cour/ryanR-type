/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** OwnerComponent
*/

#ifndef OWNERCOMPONENT_HPP_
#define OWNERCOMPONENT_HPP_

#include "../base/IComponent.hpp"

namespace ecs {

class OwnerComponent : public IComponent {
    public:
        OwnerComponent(ecs::Entity owner = 0) : _owner(owner) {};
        ~OwnerComponent() override = default;

        ecs::Entity getOwner() const { return _owner; }
        void setOwner(ecs::Entity owner) { _owner = owner; }

    private:
        ecs::Entity _owner;
};

}  // namespace ecs

#endif /* !OWNERCOMPONENT_HPP_ */