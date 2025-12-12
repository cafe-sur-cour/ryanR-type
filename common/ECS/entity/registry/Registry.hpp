/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Registry
*/

#ifndef REGISTRY_HPP_
#define REGISTRY_HPP_

#include "../../../components/base/IComponent.hpp"
#include "../componentArray/IComponentArray.hpp"
#include "../componentArray/AComponentArray.hpp"
#include <memory>
#include <unordered_map>
#include <string>

namespace ecs {

template <typename... Components> class View;
template <typename... Components> class Group;

class Registry : public std::enable_shared_from_this<Registry> {
    public:
        Registry();
        ~Registry();

        template <typename T>
        void registerComponent();

        template <typename T>
        void addComponent(Entity entityId, std::shared_ptr<T> component);

        template <typename T>
        std::shared_ptr<T> getComponent(Entity entityId) const;

        template <typename T>
        std::vector<std::shared_ptr<T>> getComponents(Entity entityId) const;

        template <typename T>
        void removeAllComponents(Entity entityId);

        template <typename T>
        void removeOneComponent(Entity entityId);

        template <typename T>
        bool hasComponent(Entity entityId) const;

        template <typename... Components>
        View<Components...> view();

        template <typename... Components>
        Group<Components...> group();

        Entity getMaxEntityId() const;

        Entity createEntity();
        void destroyEntity(Entity entityId);
    protected:
    private:
        Entity _nextEntityId;
        std::unordered_map<std::string, std::shared_ptr<IComponentArray>> _components;
};

} // namespace ecs

#include "Registry.tpp"

#endif /* !REGISTRY_HPP_ */
