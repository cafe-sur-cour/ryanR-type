/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** IRegistry
*/

#ifndef IREGISTRY_HPP_
#define IREGISTRY_HPP_

#include <memory>
#include "../../../components/base/IComponent.hpp"
#include "../../view/View.hpp"
#include "../Entity.hpp"

namespace ecs {

class IRegistry {
    public:
        virtual ~IRegistry() = default;

        template <typename T>
        void registerComponent();

        template <typename T>
        void addComponent(Entity entityId, std::shared_ptr<T> component);

        template <typename T>
        std::shared_ptr<T> getComponent(Entity entityId);

        template <typename T>
        void removeComponent(Entity entityId);

        template <typename T>
        bool hasComponent(Entity entityId);
        template <typename... Components>
        View<Components...> view();

        template <typename... Components>
        Group<Components...> group();

        size_t getMaxEntityId() const;

        virtual void removeAllComponentsWithState(ComponentState state) = 0;

        virtual size_t createEntity() = 0;
};

} // namespace ecs

#endif /* !IREGISTRY_HPP_ */
