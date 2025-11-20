# ECS POC – Tags/Filters

## Overview
Entity filtering using tags to optimize system queries and reduce unnecessary processing.

## Implementation Details
- **ECS Pattern**: Tags/Filters
- **Key Components**: HealthComponent, PositionComponent, TeamComponent (with Team enum)
- **Key Systems**: CollisionSystem (filters by Enemy tag)
- **Interaction Method**: Tag-based entity filtering for targeted processing

## Code Snippets
```cpp
enum Team { Player, Enemy };

class TeamComponent : public Component {
public:
    Team team;
    TeamComponent(Team t) : team(t) {}
};

// In CollisionSystem
if (pos && health && team && team->team == Enemy) {
    // Only process enemy entities
    if (pos->x > 50) {
        health->health -= 10;
    }
}
```

## Evaluation Criteria

### Team Preference
- Neutral - useful but not exciting
- Straightforward to understand

### Code Quality
- Clean conditional logic
- Easy to read filtering conditions

### Modularity
- Good modularity for system-specific processing
- Easy to add new tags and filters

### Flexibility
- Flexible for different entity categories
- Can combine multiple tags for complex filtering

### Performance
- Good optimization for large entity counts
- Reduces unnecessary processing

### Simplicity
- Simple to implement and understand
- Easy to debug and modify

## Pros
- Effective optimization for entity queries
- Clean separation of processing logic
- Scalable for large numbers of entities

## Cons
- Additional component overhead
- Requires careful tag management
- Not a complete interaction solution by itself

## Conclusion
- Decent optimization technique but not revolutionary
- Good complementary approach to other patterns
- Useful for performance-critical sections