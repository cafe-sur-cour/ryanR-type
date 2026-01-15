/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** EntityPartsComponent
*/

#ifndef ENTITYPARTSCOMPONENT_HPP_
#define ENTITYPARTSCOMPONENT_HPP_

#include "../base/IComponent.hpp"
#include <vector>

namespace ecs {

class EntityPartsComponent : public IComponent {
    public:
        EntityPartsComponent() = default;
        ~EntityPartsComponent() = default;

        std::vector<size_t> partIds;
};

}  // namespace ecs

#endif /* !ENTITYPARTSCOMPONENT_HPP_ */