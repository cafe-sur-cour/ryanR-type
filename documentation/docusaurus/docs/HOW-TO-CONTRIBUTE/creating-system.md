---
sidebar_position: 5
---

# Creating a New C++ System

This guide explains how to implement a new System in the ECS architecture to add game logic.

## The `ASystem` Base Class

All systems must inherit from `ecs::ASystem`.

### 1. Header File (`MySystem.hpp`)

```cpp
#pragma once
#include "../base/ASystem.hpp"

namespace ecs {

class MySystem : public ASystem {
public:
    MySystem();
    ~MySystem() = default;

    void update(
        std::shared_ptr<ResourceManager> resourceManager,
        std::shared_ptr<Registry> registry,
        float deltaTime
    ) override;
};

} // namespace ecs
```

### 2. View Iteration (`MySystem.cpp`)

The core of a system is iterating over entities that have specific components using `registry->view<T...>()`.

```cpp
#include "MySystem.hpp"
#include "../../components/permanent/TransformComponent.hpp"
#include "../../components/permanent/HealthComponent.hpp"

namespace ecs {

MySystem::MySystem() {}

void MySystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime
) {
    // 1. Create a view of entities with specific components
    auto view = registry->view<TransformComponent, HealthComponent>();

    // 2. Iterate over them
    for (auto entityId : view) {

        // 3. Retrieve components
        auto transform = registry->getComponent<TransformComponent>(entityId);
        auto health = registry->getComponent<HealthComponent>(entityId);

        // 4. Implement logic
        if (transform->getPosition().y > 1000) {
            health->applyDamage(10);
        }
    }
}

} // namespace ecs
```

## Registering Your System

Systems are typically managed by the `GameStateMachine` (GSM) or specific Game States (e.g., `PlayState`, `MenuState`).

To enable your system, you usually add it to the list of active systems in the state's initialization phase.

```cpp
// In a State class (e.g., ClientGameState.cpp)
void ClientGameState::init() {
    _systemManager->addSystem(std::make_shared<MySystem>());
}
```
