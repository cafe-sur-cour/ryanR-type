/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Registry
*/


#include "Registry.hpp"
#include "../../view/View.hpp"

namespace ecs {

template <typename T>
void Registry::registerComponent()
{
    std::string typeName = typeid(T).name();
    if (_components.find(typeName) == _components.end()) {
        _components[typeName] = std::make_shared<AComponentArray<T>>();
    }
}

template <typename T>
void Registry::addComponent(Entity entityId, std::shared_ptr<T> component)
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
std::shared_ptr<T> Registry::getComponent(Entity entityId) const
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
std::vector<std::shared_ptr<T>> Registry::getComponents(Entity entityId) const
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
void Registry::removeAllComponents(Entity entityId)
{
    const char *typeName = typeid(T).name();
    auto it = _components.find(typeName);
    if (it != _components.end()) {
        auto array = std::static_pointer_cast<AComponentArray<T>>(it->second);
        array->removeComponents(entityId);
    }
}

template <typename T>
void Registry::removeOneComponent(Entity entityId)
{
    const char *typeName = typeid(T).name();
    auto it = _components.find(typeName);
    if (it != _components.end()) {
        auto array = std::static_pointer_cast<AComponentArray<T>>(it->second);
        array->removeOneComponent(entityId);
    }
}

template <typename T>
bool Registry::hasComponent(Entity entityId) const
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
View<Components...> Registry::view()
{
    return View<Components...>(std::static_pointer_cast<Registry>(shared_from_this()));
}

template <typename... Components>
Group<Components...> Registry::group()
{
    return Group<Components...>(std::static_pointer_cast<Registry>(shared_from_this()));
}

}  // namespace ecs
