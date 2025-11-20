/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** IComponentArray
*/

#ifndef ICOMPONENTARRAY_HPP_
#define ICOMPONENTARRAY_HPP_

#include <vector>
#include "../component/IComponent.hpp"

class IComponentArray {
    public:
        virtual ~IComponentArray() = default;
        virtual void removeAllComponentsWithState(ComponentState state) = 0;
};

#endif /* !ICOMPONENTARRAY_HPP_ */
