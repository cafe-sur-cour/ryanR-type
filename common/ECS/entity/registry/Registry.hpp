/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Registry
*/

#ifndef REGISTRY_HPP_
#define REGISTRY_HPP_

#include "../componentArray/IComponentArray.hpp"
#include "../componentArray/AComponentArray.hpp"
#include <memory>
#include <unordered_map>
#include <string>
#include <functional>
#include <mutex>

namespace ecs {

template <typename... Components> class View;
template <typename... Components> class Group;

class Registry : public std::enable_shared_from_this<Registry> {
    public:
        Registry();
        explicit Registry(Entity nextEntityId);
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

        Entity getMaxEntityId() const;

        Entity createEntity();
        void destroyEntity(Entity entityId);
        void clearAllEntities();

        void setOnEntityDestroyed(std::function<void(Entity)> callback);
    protected:
    private:
        Entity _nextEntityId;
        std::unordered_map<std::string, std::shared_ptr<IComponentArray>> _components;
        std::function<void(Entity)> _onEntityDestroyed;
        mutable std::recursive_mutex _mutex;
};

} // namespace ecs

#include "Registry.tpp"

#endif /* !REGISTRY_HPP_ */
