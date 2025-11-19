# ECS POC – Event Bus

## Overview
Decoupled communication through an event system where systems publish and subscribe to events.

## Implementation Details
- **ECS Pattern**: Event Bus
- **Key Components**: HealthComponent, PositionComponent, VelocityComponent
- **Key Systems**: ProjectileSystem (publishes events), DamageSystem (subscribes to events)
- **Interaction Method**: Event publishing/subscription with type-based event handling

## Code Snippets
```cpp
class EventBus {
private:
    std::unordered_map<std::type_index, std::vector<std::function<void(Event&)>>> subscribers;

public:
    template<typename T>
    void subscribe(std::function<void(T&)> handler) {
        subscribers[typeid(T)].push_back([handler](Event& e) {
            handler(static_cast<T&>(e));
        });
    }

    template<typename T>
    void publish(T event) {
        auto& subs = subscribers[typeid(T)];
        for (auto& sub : subs) {
            sub(event);
        }
    }
};
```

## Evaluation Criteria

### Team Preference
- Interesting decoupling concept
- Felt powerful but sometimes overkill

### Code Quality
- Clean separation between systems
- Good abstraction of communication

### Modularity
- Excellent modularity with loose coupling
- Easy to add new event types and handlers

### Flexibility
- Very flexible for complex interactions
- Easy to extend with new event types

### Performance
- Event dispatch overhead
- Good for systems with varying update frequencies

### Simplicity
- Can become complex with many event types
- Harder to debug event flows

## Pros
- Clean decoupling between systems
- Flexible and extensible event system
- Good for complex game logic

## Cons
- Potentially too complex for our needs
- Difficult to debug event chains
- Performance overhead for simple interactions

## Conclusion
- Good approach but might be overkill for our current scope
- Consider for larger projects with complex interactions
- Could be simplified for our use case