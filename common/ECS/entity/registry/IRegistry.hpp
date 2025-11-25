/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** IRegistry
*/

#ifndef IREGISTRY_HPP_
#define IREGISTRY_HPP_

#include <memory>
#include "../../component/base/IComponent.hpp"

namespace ecs {

class IRegistry {
    public:
        virtual ~IRegistry() = default;

        template <typename T>
        void registerComponent();

        template <typename T>
        void addComponent(int entityId, std::shared_ptr<T> component);

        template <typename T>
        std::shared_ptr<T> getComponent(int entityId);

        template <typename T>
        void removeComponent(int entityId);

        template <typename T>
        bool hasComponent(int entityId);

        virtual void removeAllComponentsWithState(ComponentState state) = 0;
};

} // namespace ecs

#endif /* !IREGISTRY_HPP_ */
