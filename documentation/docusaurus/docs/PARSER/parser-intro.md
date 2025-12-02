---
title: Parser
---

Presentation
- **Purpose:** The parser transforms entity files (prefabs) into `ParsedEntityPrefab` objects and registers them in the `EntityPrefabManager`.
- **Location:** `common/Parser`.

Key concepts
- **Main class:** `Parser`
  - **Constructor:** `Parser(std::shared_ptr<EntityPrefabManager> prefab, ParsingType type)` — `type` is `CLIENT` or `SERVER`.
  - **Important methods:**
    - `void parseAllEntities(std::string directoryPath)`: browses a folder and calls `parseEntity` for each file.
    - `void parseEntity(std::string entityPath)`: parses an entity file and registers the prefab in the `EntityPrefabManager`.
    - `std::shared_ptr<EntityPrefabManager> getPrefabManager() const` / `void setPrefabManager(...)`: access to the prefab manager.
    - `bool isClientParsing() const` / `bool isServerParsing() const`: helpers for the `ParsingType`.
    - `bool shouldParseComponent(std::map<std::string, std::shared_ptr<FieldValue>> fields) const`: decides if a component should be parsed according to the `target` field (`"both"`, `"client"`, `"server"`).

- **Types and utilities (in `ParserParam.hpp`):**
  - `enum class FieldType { VECTOR2F, FLOAT, STRING, INT }`
  - `struct Field { std::string name; FieldType type; }`: description of an expected field for a component.
  - `using FieldValue = std::variant<math::Vector2f, float, std::string, int>`: value of a parsed field.
  - `using ComponentCreator = std::function<std::shared_ptr<ecs::IComponent>(const std::map<std::string, std::shared_ptr<FieldValue>>&)>`
  - `using ComponentAdder = std::function<void(std::shared_ptr<ecs::Registry>, ecs::Entity, std::shared_ptr<ecs::IComponent>)>`

Component definitions and expected fields
The file `ComponentParserCreators.cpp` lists known components and their required fields. Extract:

- **TransformComponent**: `target`, `position` (VECTOR2F), `scale` (VECTOR2F), `rotation` (FLOAT)
- **VelocityComponent**: `target`
- **SpeedComponent**: `target`, `speed` (FLOAT)
- **SpriteComponent**: `target`, `filePath` (STRING)
- **AnimationComponent**: `target`, `animationPath` (STRING), `frameWidth` (FLOAT), `frameHeight` (FLOAT), `frameCount` (INT), `startWidth` (FLOAT), `startHeight` (FLOAT)
- **ControllableTag**: `target`
- **PlayerTag**: `target`
- **ColliderComponent**: `target`, `size` (VECTOR2F)

Registering creators
- `Parser::instanciateComponentCreators()` calls `registerComponent<T>` for each component. `registerComponent<T>` stores:
  - a `ComponentCreator` that converts a `map<string, FieldValue>` into a concrete `std::shared_ptr<ecs::IComponent>`;
  - a `ComponentAdder` (lambda) to add that component to a `Registry` using `registry->addComponent`.

Usage example
```cpp
auto prefabManager = std::make_shared<EntityPrefabManager>();
Parser parser(prefabManager, ParsingType::CLIENT);
// Parse all files in a folder
parser.parseAllEntities("configs");
// or parse a specific file
parser.parseEntity("configs/player.json");
```

`target` behavior
- If a component's `target` field is empty -> the component is not parsed.
- `target == "both"` -> parsed on both client and server.
- `target == "client"` -> parsed only when `Parser` is constructed with `ParsingType::CLIENT`.
- `target == "server"` -> parsed only when `Parser` is constructed with `ParsingType::SERVER`.
