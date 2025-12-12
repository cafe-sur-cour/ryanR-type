/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AComponentArray template implementation
*/

#ifndef AComponentArray_TPP_
#define AComponentArray_TPP_

#include "AComponentArray.hpp"
#include <algorithm>

namespace ecs {

template <typename T>
AComponentArray<T>::AComponentArray() {
}

template <typename T>
AComponentArray<T>::~AComponentArray() {
}

template <typename T>
void AComponentArray<T>::add(Entity entityId, std::shared_ptr<T> component) {
    if (entityId >= static_cast<size_t>(_components.size()))
        _components.resize(entityId + 1);
    _components[entityId].push_back(component);
}

template <typename T>
std::shared_ptr<T> AComponentArray<T>::get(Entity entityId) const {
    if (entityId < static_cast<size_t>(_components.size()) && !_components[entityId].empty())
        return _components[entityId][0];
    return nullptr;
}

template <typename T>
std::vector<std::shared_ptr<T>> AComponentArray<T>::getAll(Entity entityId) const {
    if (entityId < static_cast<size_t>(_components.size()))
        return _components[entityId];
    return {};
}

template <typename T>
void AComponentArray<T>::removeComponents(Entity entityId) {
    if (entityId < static_cast<size_t>(_components.size()))
        _components[entityId].clear();
}

template <typename T>
void AComponentArray<T>::removeOneComponent(Entity entityId) {
    if (entityId < static_cast<size_t>(_components.size()))
        _components[entityId].pop_back();
}

template <typename T>
bool AComponentArray<T>::has(Entity entityId) const
{
    if (entityId < static_cast<size_t>(_components.size()))
        return !_components[entityId].empty();
    return false;
}

template <typename T>
Entity AComponentArray<T>::getMaxEntityId() const {
    return _components.size();
}

}  // namespace ecs

#endif /* !AComponentArray_TPP_ */
