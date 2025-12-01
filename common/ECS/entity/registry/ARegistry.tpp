/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ARegistry
*/


#include "ARegistry.hpp"

namespace ecs {

template <typename T>
void ARegistry::registerComponent()
{
    std::string typeName = typeid(T).name();
    if (_components.find(typeName) == _components.end()) {
        _components[typeName] = std::make_shared<AComponentArray<T>>();
    }
}

template <typename T>
void ARegistry::addComponent(Entity entityId, std::shared_ptr<T> component)
{
    registerComponent<T>();
    std::string typeName = typeid(T).name();
    auto it = _components.find(typeName);
    if (it != _components.end()) {
        auto array = std::static_pointer_cast<AComponentArray<T>>(it->second);
        array->add(entityId, component);
    }
}

template <typename T>
std::shared_ptr<T> ARegistry::getComponent(Entity entityId) const
{
    std::string typeName = typeid(T).name();
    auto it = _components.find(typeName);
    if (it != _components.end()) {
        auto array = std::static_pointer_cast<AComponentArray<T>>(it->second);
        return array->get(entityId);
    }
    return nullptr;
}

template <typename T>
std::vector<std::shared_ptr<T>> ARegistry::getComponents(Entity entityId) const
{
    std::string typeName = typeid(T).name();
    auto it = _components.find(typeName);
    if (it != _components.end()) {
        auto array = std::static_pointer_cast<AComponentArray<T>>(it->second);
        return array->getAll(entityId);
    }
    return {};
}

template <typename T>
void ARegistry::removeComponent(Entity entityId)
{
    const char *typeName = typeid(T).name();
    auto it = _components.find(typeName);
    if (it != _components.end()) {
        auto array = std::static_pointer_cast<AComponentArray<T>>(it->second);
        array->remove(entityId);
    }
}

template <typename T>
bool ARegistry::hasComponent(Entity entityId) const
{
    std::string typeName = typeid(T).name();
    auto it = _components.find(typeName);
    if (it != _components.end()) {
        auto array = std::static_pointer_cast<AComponentArray<T>>(it->second);
        return array->has(entityId);
    }
    return false;
}

template <typename... Components>
View<Components...> ARegistry::view()
{
    return View<Components...>(std::static_pointer_cast<ARegistry>(shared_from_this()));
}

template <typename... Components>
Group<Components...> ARegistry::group()
{
    return Group<Components...>(std::static_pointer_cast<ARegistry>(shared_from_this()));
}

} // namespace ecs
