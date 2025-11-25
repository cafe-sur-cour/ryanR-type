/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** View implementation
*/

#include "View.hpp"
#include "../entity/registry/ARegistry.hpp"
#include "../component/tags/PlayerTag.hpp"
#include "../component/temporary/MovementIntentComponent.hpp"
#include "../component/permanent/TransformComponent.hpp"

namespace ecs {

template <typename... Components>
View<Components...>::View(std::shared_ptr<ARegistry> registry)
: _registry(registry) {
    (_registry->template registerComponent<Components>(), ...);
}

template <typename... Components>
View<Components...>::Iterator::Iterator(std::shared_ptr<ARegistry> registry,
                                        size_t entityId, size_t maxEntityId)
: _registry(registry), _entityId(entityId), _maxEntityId(maxEntityId) {
}

template <typename... Components>
bool View<Components...>::Iterator::operator!=(const Iterator& other) const {
    return _entityId != other._entityId;
}

template <typename... Components>
typename View<Components...>::Iterator&
View<Components...>::Iterator::operator++() {
    do {
        ++_entityId;
    } while (_entityId < _maxEntityId && !hasAllComponents());
    return *this;
}

template <typename... Components>
size_t View<Components...>::Iterator::operator*() const {
    return _entityId;
}

template <typename... Components>
bool View<Components...>::Iterator::hasAllComponents() const {
    return (_registry->template hasComponent<Components>(_entityId) && ...);
}

template <typename... Components>
typename View<Components...>::Iterator View<Components...>::begin() {
    size_t maxEntityId = _registry->getMaxEntityId();
    size_t startId = 0;
    while (startId < maxEntityId &&
           !(_registry->template hasComponent<Components>(startId) && ...)) {
        ++startId;
    }
    return Iterator(_registry, startId, maxEntityId);
}

template <typename... Components>
typename View<Components...>::Iterator View<Components...>::end() {
    size_t maxEntityId = _registry->getMaxEntityId();
    return Iterator(_registry, maxEntityId, maxEntityId);
}

template <typename... Components>
Group<Components...>::Group(std::shared_ptr<ARegistry> registry)
: _registry(registry) {
    (_registry->template registerComponent<Components>(), ...);
}

template <typename... Components>
Group<Components...>::Iterator::Iterator(std::shared_ptr<ARegistry> registry,
                                         size_t entityId, size_t maxEntityId)
: _registry(registry), _entityId(entityId), _maxEntityId(maxEntityId) {
}

template <typename... Components>
bool Group<Components...>::Iterator::operator!=(const Iterator& other) const {
    return _entityId != other._entityId;
}

template <typename... Components>
typename Group<Components...>::Iterator&
Group<Components...>::Iterator::operator++() {
    do {
        ++_entityId;
    } while (_entityId < _maxEntityId && !hasAllComponents());
    return *this;
}

template <typename... Components>
size_t Group<Components...>::Iterator::operator*() const {
    return _entityId;
}

template <typename... Components>
bool Group<Components...>::Iterator::hasAllComponents() const {
    return (_registry->template hasComponent<Components>(_entityId) && ...);
}

template <typename... Components>
typename Group<Components...>::Iterator Group<Components...>::begin() {
    size_t maxEntityId = _registry->getMaxEntityId();
    size_t startId = 0;
    while (startId < maxEntityId &&
           !(_registry->template hasComponent<Components>(startId) && ...)) {
        ++startId;
    }
    return Iterator(_registry, startId, maxEntityId);
}

template <typename... Components>
typename Group<Components...>::Iterator Group<Components...>::end() {
    size_t maxEntityId = _registry->getMaxEntityId();
    return Iterator(_registry, maxEntityId, maxEntityId);
}

/* Explicit template instantiations for commonly used component combinations */
template class View<PlayerTag>;
template class View<MovementIntentComponent>;
template class View<PlayerTag, MovementIntentComponent>;
template class View<PlayerTag, TransformComponent>;
template class View<MovementIntentComponent, TransformComponent>;
template class Group<PlayerTag>;
template class Group<MovementIntentComponent>;
template class Group<PlayerTag, MovementIntentComponent>;

}  // namespace ecs
