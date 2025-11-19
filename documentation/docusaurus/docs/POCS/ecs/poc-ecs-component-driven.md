# ECS POC – Component-Driven

## Overview
Effects stored as temporary components that systems process and clean up.

## Implementation Details
- **ECS Pattern**: Component-Driven
- **Key Components**: HealthComponent, PositionComponent, DamageToApplyComponent (temporary)
- **Key Systems**: CollisionSystem (adds damage components), DamageSystem (processes and removes them)
- **Interaction Method**: Temporary components for effect queuing and processing

## Code Snippets
```cpp
class DamageToApplyComponent : public Component {
public:
    int damage;
    DamageToApplyComponent(int d) : damage(d) {}
};

// CollisionSystem adds damage component
components[entity->id].push_back(std::make_shared<DamageToApplyComponent>(10));

// DamageSystem processes and removes
for (auto& damage : damages) {
    health->health -= damage->damage;
}
// Remove processed components
entityComponents.erase(
    std::remove_if(entityComponents.begin(), entityComponents.end(),
        [](const std::shared_ptr<Component>& comp) {
            return std::dynamic_pointer_cast<DamageToApplyComponent>(comp) != nullptr;
        }),
    entityComponents.end()
);
```

## Evaluation Criteria

### Team Preference
- Very comfortable and intuitive approach
- Easy to understand and work with

### Code Quality
- Clean and maintainable code
- Good separation of concerns

### Modularity
- Excellent modularity - systems are independent
- Easy to add new effect types

### Flexibility
- Highly flexible for various game mechanics
- Easy to extend with new component types

### Performance
- Good performance with controlled timing
- No complex overhead

### Simplicity
- Simple to implement and understand
- Clear data flow

## Pros
- Perfect balance between modularity and cleanliness
- Simple yet powerful approach
- Easy to debug and maintain

## Cons
- Temporary component cleanup required
- Slightly more complex than direct approach

## Conclusion
- Excellent compromise for our project
- Recommended as the primary ECS pattern
- Best balance of all criteria for our team and project needs