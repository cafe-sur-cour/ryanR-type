/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** View implementation
*/

#include "View.hpp"
#include "../entity/registry/ARegistry.hpp"
#include "../component/tags/ControllableTag.hpp"
#include "../component/tags/PlayerTag.hpp"
#include "../component/temporary/MovementIntentComponent.hpp"
#include "../component/temporary/InputIntentComponent.hpp"
#include "../component/permanent/TransformComponent.hpp"
#include "../component/permanent/VelocityComponent.hpp"
#include "../component/rendering/SpriteComponent.hpp"
#include "../component/rendering/AnimationComponent.hpp"
#include "../component/permanent/ColliderComponent.hpp"
#include "../component/rendering/HitboxRenderComponent.hpp"
#include "../component/rendering/RectangleRenderComponent.hpp"

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
    for (size_t i = 0; i <= maxEntityId; ++i) {
        if ((_registry->template hasComponent<Components>(i) && ...)) {
            return Iterator(_registry, i, maxEntityId + 1);
        }
    }
    return Iterator(_registry, maxEntityId + 1, maxEntityId + 1);
}

template <typename... Components>
typename View<Components...>::Iterator View<Components...>::end() {
    size_t maxEntityId = _registry->getMaxEntityId();
    return Iterator(_registry, maxEntityId + 1, maxEntityId + 1);
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
    for (size_t i = 0; i <= maxEntityId; ++i) {
        if ((_registry->template hasComponent<Components>(i) && ...)) {
            return Iterator(_registry, i, maxEntityId + 1);
        }
    }
    return Iterator(_registry, maxEntityId + 1, maxEntityId + 1);
}

template <typename... Components>
typename Group<Components...>::Iterator Group<Components...>::end() {
    size_t maxEntityId = _registry->getMaxEntityId();
    return Iterator(_registry, maxEntityId + 1, maxEntityId + 1);
}

/* Explicit template instantiations for commonly used component combinations */
template class View<PlayerTag>;
template class View<ControllableTag>;
template class View<MovementIntentComponent>;
template class View<InputIntentComponent>;
template class View<TransformComponent>;
template class View<PlayerTag, MovementIntentComponent>;
template class View<PlayerTag, TransformComponent>;
template class View<MovementIntentComponent, TransformComponent>;
template class View<VelocityComponent, TransformComponent>;
template class View<SpriteComponent, TransformComponent>;
template class View<AnimationComponent, TransformComponent>;
template class View<TransformComponent, ColliderComponent>;
template class View<PlayerTag, TransformComponent, ColliderComponent>;
template class View<HitboxRenderComponent, TransformComponent>;
template class View<RectangleRenderComponent, TransformComponent>;
template class View<
HitboxRenderComponent, TransformComponent, ColliderComponent>;
template class Group<PlayerTag>;
template class Group<MovementIntentComponent>;
template class Group<PlayerTag, MovementIntentComponent>;

}  // namespace ecs
