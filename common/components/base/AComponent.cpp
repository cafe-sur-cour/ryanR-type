/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AComponent
*/

#include "AComponent.hpp"

namespace ecs {

AComponent::AComponent() {
}

AComponent::~AComponent() {
}

ComponentState AComponent::getState() const {
    return _state;
}

void AComponent::setState(ComponentState newState) {
    _state = newState;
}

}  // namespace ecs
