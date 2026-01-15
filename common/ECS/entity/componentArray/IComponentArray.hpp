/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** IComponentArray
*/

#ifndef ICOMPONENTARRAY_HPP_
#define ICOMPONENTARRAY_HPP_

#include "../Entity.hpp"

namespace ecs {

class IComponentArray {
    public:
        virtual ~IComponentArray() = default;
        virtual Entity getMaxEntityId() const = 0;
        virtual void removeComponents(Entity entityId) = 0;
        virtual void removeOneComponent(Entity entityId) = 0;
        virtual void clear() = 0;
};

} // namespace ecs

#endif /* !ICOMPONENTARRAY_HPP_ */
