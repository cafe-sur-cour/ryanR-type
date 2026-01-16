---
sidebar_position: 2
---

# ECS API Reference

Complete API documentation for the R-Type Entity-Component-System framework.

## Registry Class

The `Registry` is the central manager for all entities and components.

### Entity Management

#### `createEntity()`
Creates a new entity and returns its unique identifier.

```cpp
Entity entity = registry->createEntity();
```

**Returns:** `Entity` (size_t) - Unique entity identifier

**Example:**
```cpp
auto registry = std::make_shared<Registry>();
Entity player = registry->createEntity();
Entity enemy = registry->createEntity();
```

#### `destroyEntity(Entity entityId)`
Destroys an entity and removes all its components.

```cpp
void destroyEntity(Entity entityId);
```

**Parameters:**
- `entityId`: The entity to destroy

**Example:**
```cpp
Entity bullet = registry->createEntity();
// ... later
registry->destroyEntity(bullet);
```

**Callback Support:**
```cpp
registry->setOnEntityDestroyed([](Entity e) {
    std::cout << "Entity " << e << " destroyed" << std::endl;
});
```

#### `clearAllEntities()`
Removes all entities and clears all component arrays.

```cpp
void clearAllEntities();
```

**Example:**
```cpp
// Clean up when changing game state
registry->clearAllEntities();
```

#### `getMaxEntityId()`
Returns the highest entity ID currently allocated.

```cpp
Entity getMaxEntityId() const;
```

**Returns:** `Entity` - Maximum entity ID

### Component Management

#### `registerComponent<T>()`
Registers a component type with the registry (usually called automatically).

```cpp
template <typename T>
void registerComponent();
```

**Example:**
```cpp
registry->registerComponent<TransformComponent>();
registry->registerComponent<HealthComponent>();
```

**Note:** This is typically called automatically when adding a component.

#### `addComponent<T>(Entity, std::shared_ptr<T>)`
Adds a component to an entity.

```cpp
template <typename T>
void addComponent(Entity entityId, std::shared_ptr<T> component);
```

**Parameters:**
- `entityId`: Target entity
- `component`: Shared pointer to the component

**Example:**
```cpp
Entity player = registry->createEntity();

// Add transform component
auto transform = std::make_shared<TransformComponent>(100.0f, 200.0f);
registry->addComponent<TransformComponent>(player, transform);

// Add health component
auto health = std::make_shared<HealthComponent>(100);
registry->addComponent<HealthComponent>(player, health);
```

**Thread Safety:** Uses mutex lock, safe for concurrent access.

#### `getComponent<T>(Entity)`
Retrieves the first component of type T from an entity.

```cpp
template <typename T>
std::shared_ptr<T> getComponent(Entity entityId) const;
```

**Returns:** `std::shared_ptr<T>` - Pointer to component, or `nullptr` if not found

**Example:**
```cpp
auto transform = registry->getComponent<TransformComponent>(player);
if (transform) {
    transform->x += 10.0f;
    transform->y += 5.0f;
}
```

#### `getComponents<T>(Entity)`
Retrieves all components of type T from an entity (for multi-component support).

```cpp
template <typename T>
std::vector<std::shared_ptr<T>> getComponents(Entity entityId) const;
```

**Returns:** `std::vector<std::shared_ptr<T>>` - Vector of all matching components

**Example:**
```cpp
// Entity can have multiple damage components queued
auto damages = registry->getComponents<DamageToApplyComponent>(entity);
for (auto& damage : damages) {
    totalDamage += damage->damage;
}
```

#### `hasComponent<T>(Entity)`
Checks if an entity has a specific component type.

```cpp
template <typename T>
bool hasComponent(Entity entityId) const;
```

**Returns:** `bool` - True if entity has the component

**Example:**
```cpp
if (registry->hasComponent<HealthComponent>(entity)) {
    auto health = registry->getComponent<HealthComponent>(entity);
    health->currentHealth -= 10;
}
```

#### `removeAllComponents<T>(Entity)`
Removes all components of type T from an entity.

```cpp
template <typename T>
void removeAllComponents(Entity entityId);
```

**Example:**
```cpp
// Remove all damage components after processing
registry->removeAllComponents<DamageToApplyComponent>(entity);
```

#### `removeOneComponent<T>(Entity)`
Removes the first component of type T from an entity.

```cpp
template <typename T>
void removeOneComponent(Entity entityId);
```

**Example:**
```cpp
// Remove one damage component after processing
registry->removeOneComponent<DamageToApplyComponent>(entity);
```

### View System

#### `view<Components...>()`
Creates a view to iterate entities that have ALL specified components.

```cpp
template <typename... Components>
View<Components...> view();
```

**Returns:** `View<Components...>` - Iterable view object

**Example:**
```cpp
// Iterate all entities with Transform AND Velocity
auto view = registry->view<TransformComponent, VelocityComponent>();

for (Entity entity : view) {
    auto transform = registry->getComponent<TransformComponent>(entity);
    auto velocity = registry->getComponent<VelocityComponent>(entity);
    
    transform->x += velocity->vx * deltaTime;
    transform->y += velocity->vy * deltaTime;
}
```

**Multiple Component Types:**
```cpp
// Entities must have ALL three components
auto renderView = registry->view<TransformComponent, SpriteComponent, RenderableTag>();

for (Entity entity : renderView) {
    // Render only visible entities
}
```

**Performance:** Views use lazy evaluation and iterate only matching entities.

## View Class

Iterator for entities with specific component combinations.

### Iteration

```cpp
View<Components...> view = registry->view<Transform, Sprite>();

// Range-based for loop (recommended)
for (Entity entity : view) {
    // Process entity
}

// Traditional iterator
for (auto it = view.begin(); it != view.end(); ++it) {
    Entity entity = *it;
}
```

### Member Functions

#### `begin()`
Returns iterator to the first matching entity.

```cpp
auto begin() -> iterator;
```

#### `end()`
Returns iterator past the last matching entity.

```cpp
auto end() -> iterator;
```

## Component Base Classes

### IComponent

Base interface for all components.

```cpp
class IComponent {
public:
    virtual ~IComponent() = default;
};
```

**Usage:**
```cpp
class MyComponent : public IComponent {
public:
    // Your data members
    int value;
    std::string name;
};
```

## System Base Classes

### ASystem

Abstract base class for all systems.

```cpp
class ASystem {
public:
    virtual ~ASystem() = default;
    
    virtual void update(float deltaTime) = 0;
    
    void setRegistry(std::shared_ptr<Registry> registry);
    
protected:
    std::shared_ptr<Registry> registry;
};
```

**Creating a System:**
```cpp
class MySystem : public ASystem {
public:
    void update(float deltaTime) override {
        auto view = registry->view<MyComponent>();
        
        for (Entity entity : view) {
            auto comp = registry->getComponent<MyComponent>(entity);
            // Process component
        }
    }
};
```

**System Setup:**
```cpp
auto system = std::make_shared<MySystem>();
system->setRegistry(registry);

// In game loop
system->update(deltaTime);
```

## Common Component Types

### TransformComponent

Position, rotation, and scale in 2D space.

```cpp
class TransformComponent : public IComponent {
public:
    float x, y;           // Position
    float rotation;       // Rotation in radians
    float scaleX, scaleY; // Scale factors
    
    TransformComponent(float x = 0, float y = 0);
};
```

### VelocityComponent

Movement speed and direction.

```cpp
class VelocityComponent : public IComponent {
public:
    float vx, vy;  // Velocity in x and y
    
    VelocityComponent(float vx = 0, float vy = 0);
};
```

### SpriteComponent

Visual representation with texture.

```cpp
class SpriteComponent : public IComponent {
public:
    std::string texturePath;
    sf::Sprite sprite;
    int layer;  // Rendering layer
    
    SpriteComponent(const std::string& path);
};
```

### HealthComponent

Hit points and damage state.

```cpp
class HealthComponent : public IComponent {
public:
    int currentHealth;
    int maxHealth;
    bool invincible;
    
    HealthComponent(int maxHp);
    bool isDead() const;
};
```

### ColliderComponent

Collision detection bounds.

```cpp
class ColliderComponent : public IComponent {
public:
    float width, height;
    bool isTrigger;  // Trigger or solid collider
    
    ColliderComponent(float w, float h);
};
```

### Tags

Lightweight marker components for filtering.

```cpp
class PlayerTag : public IComponent {};
class EnemyTag : public IComponent {};
class BulletTag : public IComponent {};
class LocalPlayerTag : public IComponent {};
```

**Usage:**
```cpp
// Mark as enemy
registry->addComponent<EnemyTag>(entity, std::make_shared<EnemyTag>());

// Filter enemies only
auto enemies = registry->view<TransformComponent, EnemyTag>();
```

## Network Synchronization Components

### NetworkComponent

Tracks network synchronization state.

```cpp
class NetworkComponent : public IComponent {
public:
    bool dirty;           // Needs network update
    uint32_t version;     // Version for reconciliation
    uint8_t clientId;     // Owning client
    
    NetworkComponent();
};
```

**Delta Encoding Pattern:**
```cpp
auto networkView = registry->view<TransformComponent, NetworkComponent>();

for (Entity entity : networkView) {
    auto transform = registry->getComponent<TransformComponent>(entity);
    auto network = registry->getComponent<NetworkComponent>(entity);
    
    if (network->dirty) {
        sendTransformUpdate(entity, transform);
        network->dirty = false;
        network->version++;
    }
}
```

## Entity Factory

Helper class for creating common entity types.

### `createPlayer(Registry&, float x, float y)`

```cpp
static Entity createPlayer(
    std::shared_ptr<Registry> registry,
    float x,
    float y
);
```

**Example:**
```cpp
Entity player = EntityFactory::createPlayer(registry, 100, 200);
```

### `createEnemy(Registry&, const std::string& type)`

```cpp
static Entity createEnemy(
    std::shared_ptr<Registry> registry,
    const std::string& type
);
```

**Example:**
```cpp
Entity enemy = EntityFactory::createEnemy(registry, "basic_enemy");
```

## Threading & Concurrency

### Thread Safety Guarantees

The Registry uses a recursive mutex for thread-safe operations:

```cpp
// Safe from multiple threads
std::thread t1([&]() { registry->addComponent<A>(e1, comp1); });
std::thread t2([&]() { registry->addComponent<B>(e2, comp2); });
```

### Best Practices

**DO:** Read different component types in parallel
```cpp
// Thread 1
auto view1 = registry->view<TransformComponent>();

// Thread 2 (different component type)
auto view2 = registry->view<HealthComponent>();
```

**DON'T:** Modify entity list while iterating
```cpp
// WRONG - modifying while iterating
for (Entity e : view) {
    registry->destroyEntity(e);  // Dangerous!
}

// CORRECT - use command buffer
std::vector<Entity> toDestroy;
for (Entity e : view) {
    toDestroy.push_back(e);
}
for (Entity e : toDestroy) {
    registry->destroyEntity(e);
}
```

## Serialization

### Entity Serialization

```cpp
// Serialize entity state
nlohmann::json serializeEntity(Entity entity, Registry& registry) {
    json data;
    data["id"] = entity;
    
    if (registry.hasComponent<TransformComponent>(entity)) {
        auto t = registry.getComponent<TransformComponent>(entity);
        data["transform"] = {{"x", t->x}, {"y", t->y}};
    }
    
    // ... other components
    return data;
}
```

### Component Snapshot

```cpp
// Take snapshot of all entities with network components
std::vector<json> takeSnapshot(Registry& registry) {
    std::vector<json> entities;
    
    auto view = registry.view<NetworkComponent>();
    for (Entity e : view) {
        entities.push_back(serializeEntity(e, registry));
    }
    
    return entities;
}
```

## Performance Tips

### Component Design

```cpp
// GOOD - Simple, cache-friendly
class PositionComponent {
    float x, y;
};

// BAD - Heavy, not cache-friendly
class PositionComponent {
    std::vector<Transform> history;
    std::map<std::string, Callback> events;
};
```

### View Optimization

```cpp
// GOOD - Iterate once with multi-component view
auto view = registry->view<A, B, C>();
for (Entity e : view) {
    auto a = registry->getComponent<A>(e);
    auto b = registry->getComponent<B>(e);
    auto c = registry->getComponent<C>(e);
    // Process all together
}

// BAD - Multiple iterations
for (Entity e : registry->view<A>()) {
    if (registry->hasComponent<B>(e) && registry->hasComponent<C>(e)) {
        // Inefficient filtering
    }
}
```

### Memory Management

```cpp
// Components use shared_ptr for automatic memory management
auto comp = std::make_shared<MyComponent>(args);
registry->addComponent<MyComponent>(entity, comp);

// No manual cleanup needed
// Destroyed automatically when entity is destroyed
```

## Code Examples

### Complete Entity Creation

```cpp
// Create a player entity with all components
Entity createPlayerEntity(std::shared_ptr<Registry> registry) {
    Entity player = registry->createEntity();
    
    // Transform
    auto transform = std::make_shared<TransformComponent>(100, 200);
    registry->addComponent<TransformComponent>(player, transform);
    
    // Sprite
    auto sprite = std::make_shared<SpriteComponent>("player.png");
    registry->addComponent<SpriteComponent>(player, sprite);
    
    // Health
    auto health = std::make_shared<HealthComponent>(100);
    registry->addComponent<HealthComponent>(player, health);
    
    // Velocity
    auto velocity = std::make_shared<VelocityComponent>(0, 0);
    registry->addComponent<VelocityComponent>(player, velocity);
    
    // Collider
    auto collider = std::make_shared<ColliderComponent>(32, 32);
    registry->addComponent<ColliderComponent>(player, collider);
    
    // Tag as player
    registry->addComponent<PlayerTag>(player, std::make_shared<PlayerTag>());
    
    return player;
}
```

### System Implementation

```cpp
class DamageSystem : public ASystem {
public:
    void update(float deltaTime) override {
        // Process all entities with health and pending damage
        auto view = registry->view<HealthComponent, DamageToApplyComponent>();
        
        for (Entity entity : view) {
            auto health = registry->getComponent<HealthComponent>(entity);
            auto damage = registry->getComponent<DamageToApplyComponent>(entity);
            
            // Apply damage
            health->currentHealth -= damage->damage;
            
            // Check for death
            if (health->isDead()) {
                registry->addComponent<DestroyEntityComponent>(entity,
                    std::make_shared<DestroyEntityComponent>());
            }
            
            // Remove processed damage
            registry->removeOneComponent<DamageToApplyComponent>(entity);
        }
    }
};
```

---

For more information, see:
- [ECS Introduction](./ecs-intro.md)
- [ECS POCs](../POCS/ecs/pocs-ecs.md)
- [Project Architecture](../INTRO/project_architecure.md)
