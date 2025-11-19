/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AComponentArray template implementation
*/

#ifndef AComponentArray_TPP_
#define AComponentArray_TPP_

#include "AComponentArray.hpp"

template <typename T>
AComponentArray<T>::AComponentArray() {
}

template <typename T>
AComponentArray<T>::~AComponentArray() {
}

template <typename T>
void AComponentArray<T>::add(int entityId, std::shared_ptr<T> component) {
    if (entityId >= static_cast<int>(_components.size()))
        _components.resize(entityId + 1);
    _components[entityId] = component;
}

template <typename T>
std::shared_ptr<T> AComponentArray<T>::get(int entityId) {
    if (entityId < static_cast<int>(_components.size()))
        return _components[entityId];
    return nullptr;
}

template <typename T>
void AComponentArray<T>::remove(int entityId) {
    if (entityId < static_cast<int>(_components.size()))
        _components[entityId] = nullptr;
}

template <typename T>
bool AComponentArray<T>::has(int entityId) const
{
    if (entityId < static_cast<int>(_components.size()))
        return _components[entityId] != nullptr;
    return false;
}

template <typename T>
void AComponentArray<T>::removeAllComponentsWithState(ComponentState state) {
    for (auto& comp : _components) {
        if (comp && comp->getState() == state) {
            comp = nullptr;
        }
    }
}

#endif /* !AComponentArray_TPP_ */
