---
title: Parser
---


## Presentation

- **Purpose:** The parser system transforms configuration files (entity prefabs and maps) into game entities and registers them in the appropriate managers.
- **Location:** `common/Parser`.
- **Components:**
  - Entity Parser
  - Map Parser
  - Component Parsers (auto-registered via macros)

## Key Concepts

### Main class: `Parser`

- **Constructor:**
  ```cpp
  Parser(std::shared_ptr<EntityPrefabManager> prefab, ParsingType type, std::shared_ptr<ecs::Registry> registry)
  ```
  - `type` is `CLIENT` or `SERVER`.
- **Important methods:**
  - `void parseAllEntities(std::string directoryPath)`: Browse a folder and call `parseEntity` for each file.
  - `void parseEntity(std::string entityPath)`: Parse an entity file and register the prefab in the `EntityPrefabManager`.
  - `void parseMapFromFile(const std::string& filePath)`: Parse a map file and create entities in the game world.
  - `std::shared_ptr<EntityPrefabManager> getPrefabManager() const` / `void setPrefabManager(...)`: Access to the prefab manager.
  - `bool isClientParsing() const` / `bool isServerParsing() const`: Helpers for the `ParsingType`.
  - `bool shouldParseComponent(std::map<std::string, std::shared_ptr<FieldValue>> fields) const`: Decides if a component should be parsed according to the `target` field (`"both"`, `"client"`, `"server"`).

### Types and utilities (in `ParserParam.hpp`):

- `enum class FieldType { VECTOR2F, FLOAT, STRING, INT, BOOL, OBJECT, JSON, UNDEFINED }`
- `struct Field { std::string name; FieldType type; bool optional = false; std::shared_ptr<FieldValue> defaultValue = nullptr; }`: Description of an expected field for a component.
- `using FieldValue = std::variant<math::Vector2f, float, std::string, int, bool, FieldValueMap, nlohmann::json>`: Value of a parsed field.
- `using ComponentCreator = std::function<std::shared_ptr<ecs::IComponent>(const std::map<std::string, std::shared_ptr<FieldValue>>&)>`
- `using ComponentAdder = std::function<void(std::shared_ptr<ecs::Registry>, ecs::Entity, std::shared_ptr<ecs::IComponent>)>`

## Component definitions and expected fields

The list of available components and their fields is defined by the code via macros (`REGISTER_COMPONENT`, `REGISTER_TAG_COMPONENT`).
Here are some key components (see the code for the full list):

- **TransformComponent**: `target` (STRING), `position` (VECTOR2F), `scale` (VECTOR2F, optional, default: (1,1)), `rotation` (FLOAT, optional, default: 0)
- **VelocityComponent**: `target` (STRING)
- **SpeedComponent**: `target` (STRING), `speed` (FLOAT)
- **SpriteComponent**: `target` (STRING), `filePath` (STRING)
- **AnimationComponent**: `target` (STRING), `states` (JSON), `initialState` (STRING), `transitions` (JSON, optional, default: empty array)
- **ControllableTag**: `target` (STRING)
- **PlayerTag**: `target` (STRING)
- **ColliderComponent**: `target` (STRING), `offset` (VECTOR2F, optional, default: (0,0)), `size` (VECTOR2F), `type` (STRING)

> **Note:** Many other components and tags are available. See the codebase (look for `REGISTER_COMPONENT` and `REGISTER_TAG_COMPONENT`) for the full up-to-date list and their fields.

## Registering creators

Component registration is automatic via macros. Each component is registered with:
- a `ComponentCreator` that converts a `map<string, FieldValue>` into a concrete `std::shared_ptr<ecs::IComponent>`
- a `ComponentAdder` (lambda) to add that component to a `Registry` using `registry->addComponent`

## Usage example

```cpp
auto prefabManager = std::make_shared<EntityPrefabManager>();
auto registry = std::make_shared<ecs::Registry>();
Parser parser(prefabManager, ParsingType::CLIENT, registry);

// Parse all entity files in a folder
parser.parseAllEntities("configs/entities");

// Parse a specific entity file
parser.parseEntity("configs/player.json");

// Parse a map file
parser.parseMapFromFile("configs/maps/level1.json");
```

## Map Parser Integration

The Parser class also integrates map parsing capabilities through the `MapParser` component. Map files define level layouts, backgrounds, and enemy waves. See the [Map Parser documentation](./map-parser.md) for detailed information about map file formats and usage.

## Map Parser Integration

The Parser class also integrates map parsing capabilities through the `MapParser` component. Map files define level layouts, backgrounds, and enemy waves. See the [Map Parser documentation](./map-parser.md) for detailed information about map file formats and usage.
