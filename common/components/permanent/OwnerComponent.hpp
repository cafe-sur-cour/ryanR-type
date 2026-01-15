/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** OwnerComponent
*/

#ifndef OWNERCOMPONENT_HPP_
#define OWNERCOMPONENT_HPP_

#include "../base/AComponent.hpp"

namespace ecs {

using Entity = size_t;

class OwnerComponent : public AComponent {
    public:
        OwnerComponent(Entity owner = 0) : _owner(owner) {};
        ~OwnerComponent() override = default;

        Entity getOwner() const { return _owner; }
        void setOwner(Entity owner) { _owner = owner; }

    private:
        Entity _owner;
};

}  // namespace ecs

#endif /* !OWNERCOMPONENT_HPP_ */