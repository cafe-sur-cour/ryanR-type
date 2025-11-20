# ECS POC – Direct System

## Overview
Direct system interactions where systems call each other or access shared data for immediate interactions.

## Implementation Details
- **ECS Pattern**: Direct
- **Key Components**: HealthComponent, PositionComponent, VelocityComponent
- **Key Systems**: CollisionSystem (handles collision detection and damage application)
- **Interaction Method**: Direct method calls and shared data access

## Code Snippets
```cpp
class CollisionSystem : public System {
public:
    void update(std::vector<std::shared_ptr<Entity>>& entities,
                std::unordered_map<int, std::vector<std::shared_ptr<Component>>>& components) override {
        for (auto& entity : entities) {
            // Direct collision detection and damage
            if (pos->x > 50) {
                health->health -= 10; // Direct modification
            }
        }
    }
};
```

## Evaluation Criteria

### Team Preference
- Familiar approach but felt outdated
- Easy to understand initially but became messy

### Code Quality
- Hardcoded logic makes it difficult to maintain
- Poor separation of concerns

### Modularity
- Very low modularity - everything coupled together
- Hard to modify or extend without affecting other parts

### Flexibility
- Limited flexibility for new mechanics
- Changes require touching multiple parts of the code

### Performance
- Good performance for simple cases
- No overhead from complex patterns

### Simplicity
- Simple to implement initially
- Becomes complex as features are added

## Pros
- Straightforward and easy to start with
- No complex abstractions to learn

## Cons
- Too hardcoded and not clean
- Poor modularity makes it hard to maintain
- Difficult to test individual components

## Conclusion
- Not suitable for our project due to lack of modularity
- Better for very simple prototypes only
- Avoid for any serious game development