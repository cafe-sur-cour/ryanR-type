/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** IComponentArray
*/

#ifndef ICOMPONENTARRAY_HPP_
#define ICOMPONENTARRAY_HPP_

#include "../../component/base/IComponent.hpp"

namespace ecs {

class IComponentArray {
    public:
        virtual ~IComponentArray() = default;
        virtual void removeAllComponentsWithState(ComponentState state) = 0;
};

} // namespace ecs

#endif /* !ICOMPONENTARRAY_HPP_ */
