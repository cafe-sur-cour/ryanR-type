---
sidebar_position: 2
---

# Coding Conventions

Code style and best practices for the R-Type project.

## C++ Style Guide

### Naming Conventions

#### Files
```cpp
// Classes: PascalCase
MyClass.cpp
MyClass.hpp

// Functions/utilities: camelCase
utilityFunctions.cpp
utilityFunctions.hpp
```

#### Classes and Structs
```cpp
// PascalCase for class names
class PlayerEntity { };
class GameStateMachine { };

// PascalCase for struct names
struct TransformComponent { };
struct Vector2D { };
```

#### Functions and Methods
```cpp
// camelCase for functions
void updatePosition();
int calculateDamage();

// Getters/setters
float getSpeed() const;
void setSpeed(float speed);
```

#### Variables
```cpp
// camelCase for local variables
int playerHealth = 100;
std::string userName = "Player";

// camelCase for member variables
class Player {
private:
    int currentHealth;
    float movementSpeed;
};

// Or prefix with underscore
class Player {
private:
    int _currentHealth;
    float _movementSpeed;
};

// UPPER_CASE for constants
const int MAX_PLAYERS = 4;
constexpr float GRAVITY = 9.81f;
```

#### Namespaces
```cpp
// lowercase
namespace ecs { }
namespace rserv { }
namespace constants { }
```

#### Enums
```cpp
// PascalCase for enum name
enum class PlayerState {
    Idle,        // PascalCase for values
    Walking,
    Jumping
};

// Usage
PlayerState state = PlayerState::Idle;
```

### Code Formatting

#### Indentation
- **4 spaces** (no tabs)
- Consistent indentation level

```cpp
void example() {
    if (condition) {
        doSomething();
        doAnotherThing();
    }
}
```

#### Braces
Opening brace on same line (K&R style):

```cpp
// Functions
void function() {
    // code
}

// Conditionals
if (condition) {
    // code
} else {
    // code
}

// Loops
for (int i = 0; i < 10; i++) {
    // code
}

// Classes
class MyClass {
public:
    // members
};
```

#### Spacing

```cpp
// Operators: spaces around binary operators
int result = a + b * c;
bool isValid = (x > 0) && (y < 100);

// Function calls: no space before parenthesis
function(arg1, arg2);
myObject.method(value);

// Control structures: space after keyword
if (condition) { }
while (running) { }
for (int i = 0; i < n; i++) { }

// Commas: space after
function(arg1, arg2, arg3);
int array[] = {1, 2, 3, 4};

// Pointers/References: * and & with type
int* pointer;
const std::string& reference;
```

#### Line Length
- Maximum **100 characters** per line
- Break long lines logically

```cpp
// Long function calls
myObject->doSomethingWithALongName(
    argument1,
    argument2,
    argument3
);

// Long conditions
if (veryLongCondition1 &&
    veryLongCondition2 &&
    veryLongCondition3) {
    // code
}
```

### Comments

#### Single-line Comments
```cpp
// Use // for single-line comments
int value = 42;  // Inline comment
```

#### Multi-line Comments
```cpp
/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** MyClass
*/
```

#### Documentation Comments
```cpp
/**
 * @brief Calculates the distance between two points
 * 
 * @param p1 First point
 * @param p2 Second point
 * @return float Distance between points
 */
float calculateDistance(const Vector2D& p1, const Vector2D& p2);
```

### Header Guards

Use `#ifndef` / `#define`:

```cpp
#ifndef MYCLASS_HPP_
#define MYCLASS_HPP_

// Header content

#endif /* !MYCLASS_HPP_ */
```

**Format:** `FILENAME_HPP_` in UPPER_CASE

### Include Order

```cpp
// 1. Project header (for .cpp files)
#include "MyClass.hpp"

// 2. System headers
#include <iostream>
#include <vector>
#include <memory>

// 3. Third-party libraries
#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

// 4. Project headers
#include "../common/ECS/Registry.hpp"
#include "Utils.hpp"
```

### Modern C++ Features

#### Use Smart Pointers
```cpp
// Preferred
auto player = std::make_shared<Player>();
auto data = std::make_unique<GameData>();

// Avoid
Player* player = new Player();  // Manual memory management
```

#### Use Auto for Complex Types
```cpp
// Good
auto registry = std::make_shared<Registry>();
auto texture = resourceManager.getTexture("player");

// But be explicit when clarity matters
int count = 5;  // Not: auto count = 5;
```

#### Range-Based For Loops
```cpp
// Preferred
for (const auto& item : container) {
    // process item
}

// Avoid
for (size_t i = 0; i < container.size(); i++) {
    auto item = container[i];
}
```

#### Const Correctness
```cpp
class MyClass {
public:
    // Const methods don't modify object
    int getValue() const { return _value; }
    
    // Non-const methods can modify
    void setValue(int value) { _value = value; }
    
private:
    int _value;
};

// Const references for large objects
void processData(const std::vector<int>& data);
```

#### Nullptr over NULL
```cpp
// Preferred
Entity* entity = nullptr;

// Avoid
Entity* entity = NULL;
```

## ECS Best Practices

### Components
- **Data only**, no logic
- Small and focused
- Inherit from `IComponent`

```cpp
// GOOD
class TransformComponent : public IComponent {
public:
    float x, y;
    float rotation;
    
    TransformComponent(float x = 0, float y = 0, float rot = 0)
        : x(x), y(y), rotation(rot) {}
};

// BAD - has logic
class TransformComponent : public IComponent {
public:
    float x, y;
    
    void update(float dt) {  // NO! Logic in component
        x += speed * dt;
    }
};
```

### Systems
- **Logic only**, operate on components
- Use views for iteration
- Inherit from `ASystem`

```cpp
class MovementSystem : public ASystem {
public:
    void update(float deltaTime) override {
        auto view = registry->view<TransformComponent, VelocityComponent>();
        
        for (Entity entity : view) {
            auto transform = registry->getComponent<TransformComponent>(entity);
            auto velocity = registry->getComponent<VelocityComponent>(entity);
            
            transform->x += velocity->vx * deltaTime;
            transform->y += velocity->vy * deltaTime;
        }
    }
};
```

### Entity Creation
- Use factory methods
- Add components in logical order
- Use shared_ptr for components

```cpp
Entity createPlayer(std::shared_ptr<Registry> registry, float x, float y) {
    Entity player = registry->createEntity();
    
    registry->addComponent<TransformComponent>(player,
        std::make_shared<TransformComponent>(x, y));
    registry->addComponent<SpriteComponent>(player,
        std::make_shared<SpriteComponent>("player.png"));
    registry->addComponent<HealthComponent>(player,
        std::make_shared<HealthComponent>(100));
    
    return player;
}
```

## Error Handling

### Use Exceptions for Errors
```cpp
// Throw exceptions for error conditions
if (!file.is_open()) {
    throw std::runtime_error("Failed to open file: " + filename);
}

// Catch specific exceptions
try {
    loadConfig("settings.json");
} catch (const std::runtime_error& e) {
    std::cerr << "Config error: " << e.what() << std::endl;
} catch (const err::IError& e) {
    std::cerr << "Game error: " << e.getDetails() << std::endl;
}
```

### Validate Input
```cpp
void setPort(uint16_t port) {
    if (port < 1024 || port > 65535) {
        throw std::invalid_argument("Port must be between 1024 and 65535");
    }
    _port = port;
}
```

### Check Pointers
```cpp
auto component = registry->getComponent<HealthComponent>(entity);
if (component) {
    component->currentHealth -= damage;
} else {
    // Handle missing component
    std::cerr << "Entity " << entity << " has no HealthComponent" << std::endl;
}
```

## Performance Guidelines

### Pass by Reference
```cpp
// Large objects: pass by const reference
void processEntity(const Entity& entity);
void loadTexture(const std::string& path);

// Small objects (primitives): pass by value
void setPosition(float x, float y);
void setId(int id);
```

### Reserve Vector Capacity
```cpp
std::vector<Entity> entities;
entities.reserve(1000);  // Avoid reallocations

for (int i = 0; i < 1000; i++) {
    entities.push_back(createEntity());
}
```

### Avoid Unnecessary Copies
```cpp
// GOOD - reference
for (const auto& entity : entities) {
    processEntity(entity);
}

// BAD - copies each entity
for (auto entity : entities) {
    processEntity(entity);
}
```

### Use Emplace
```cpp
// Preferred - construct in place
vector.emplace_back(arg1, arg2);
map.emplace(key, value);

// Less efficient - constructs then copies
vector.push_back(Object(arg1, arg2));
```

## File Organization

### Header Files (.hpp)
```cpp
/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** MyClass
*/

#ifndef MYCLASS_HPP_
#define MYCLASS_HPP_

// Includes
#include <memory>
#include "OtherClass.hpp"

// Namespace
namespace myproject {

// Class declaration
class MyClass {
public:
    MyClass();
    ~MyClass();
    
    void publicMethod();
    
private:
    void privateMethod();
    
    int _memberVariable;
};

}  // namespace myproject

#endif /* !MYCLASS_HPP_ */
```

### Implementation Files (.cpp)
```cpp
/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** MyClass
*/

#include "MyClass.hpp"

#include <iostream>

namespace myproject {

MyClass::MyClass() : _memberVariable(0) {
    // Constructor
}

MyClass::~MyClass() {
    // Destructor
}

void MyClass::publicMethod() {
    // Implementation
}

void MyClass::privateMethod() {
    // Implementation
}

}  // namespace myproject
```

## CMakeLists.txt Conventions

```cmake
# Project definition
project(MyProject)

# Source files
set(SOURCES
    main.cpp
    MyClass.cpp
    Utils.cpp
)

# Executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../common
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE sfml-graphics
    PRIVATE ECS
    PRIVATE Network
)
```

## Git Commit Messages

Follow the project's commit conventions (see [Git Workflow](./git-workflow.md) for details).

**Format:**
```
<type>(<scope>): <subject>

<body>

<footer>
```

**Types:** feat, fix, docs, style, refactor, test, chore

## Code Review Checklist

Before submitting a PR:

- [ ] Code follows style guidelines
- [ ] No compiler warnings
- [ ] All tests pass
- [ ] New code has tests
- [ ] Documentation updated
- [ ] No memory leaks (checked with valgrind/sanitizers)
- [ ] Commit messages follow conventions
- [ ] PR description is clear

## Additional Resources

- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/)
- [Modern C++ Best Practices](https://github.com/cpp-best-practices/cppbestpractices)
- [SFML Documentation](https://www.sfml-dev.org/documentation/)

---

Consistent code style makes collaboration easier and code more maintainable!
