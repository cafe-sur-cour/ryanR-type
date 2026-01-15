/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** CompositeEntityComponent
*/

#ifndef COMPOSITEENTITYCOMPONENT_HPP_
#define COMPOSITEENTITYCOMPONENT_HPP_

#include "../base/IComponent.hpp"
#include <cstddef>

namespace ecs {

class CompositeEntityComponent : public IComponent {
    public:
        CompositeEntityComponent(size_t parent_id) : parentId(parent_id) {};
        ~CompositeEntityComponent() = default;

        size_t getParentId() const { return parentId; }
        void setParentId(size_t id) { parentId = id; }

    private:
        size_t parentId;
};

}  // namespace ecs

#endif /* !COMPOSITEENTITYCOMPONENT_HPP_ */