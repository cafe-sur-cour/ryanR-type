/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AComposantType template implementation
*/

#ifndef AComposantType_TPP_
#define AComposantType_TPP_

#include "ComposantType.hpp"

template <typename T>
AComposantType<T>::AComposantType() {
}

template <typename T>
AComposantType<T>::~AComposantType() {
}

template <typename T>
void AComposantType<T>::add(int entityId, std::shared_ptr<T> component) {
    if (entityId >= static_cast<int>(_components.size()))
        _components.resize(entityId + 1);
    _components[entityId] = component;
}

template <typename T>
std::shared_ptr<T> AComposantType<T>::get(int entityId) {
    if (entityId < static_cast<int>(_components.size()))
        return _components[entityId];
    return nullptr;
}

template <typename T>
void AComposantType<T>::remove(int entityId) {
    if (entityId < static_cast<int>(_components.size()))
        _components[entityId] = nullptr;
}

template <typename T>
bool AComposantType<T>::has(int entityId) const
{
    if (entityId < static_cast<int>(_components.size()))
        return _components[entityId] != nullptr;
    return false;
}

#endif /* !AComposantType_TPP_ */
