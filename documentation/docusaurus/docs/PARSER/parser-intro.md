---
title: Parser
---

Présentation
- **But :** Le parser transforme des fichiers d'entité (préfabs) en objets `ParsedEntityPrefab` et les enregistre dans le `EntityPrefabManager`.
- **Emplacement :** `common/Parser`.

Concepts clés
- **Classe principale :** `Parser`
  - **Constructeur :** `Parser(std::shared_ptr<EntityPrefabManager> prefab, ParsingType type)` — `type` est `CLIENT` ou `SERVER`.
  - **Méthodes importantes :**
    - `void parseAllEntities(std::string directoryPath)` : parcourt un dossier et appelle `parseEntity` pour chaque fichier.
    - `void parseEntity(std::string entityPath)` : parse un fichier d'entité et enregistre le prefab dans le `EntityPrefabManager`.
    - `std::shared_ptr<EntityPrefabManager> getPrefabManager() const` / `void setPrefabManager(...)` : accès au gestionnaire de préfabs.
    - `bool isClientParsing() const` / `bool isServerParsing() const` : helpers pour le `ParsingType`.
    - `bool shouldParseComponent(std::map<std::string, std::shared_ptr<FieldValue>> fields) const` : décide si un composant doit être parsé selon le champ `target` (`"both"`, `"client"`, `"server"`).

- **Types et utilitaires (dans `ParserParam.hpp`) :**
  - `enum class FieldType { VECTOR2F, FLOAT, STRING, INT }`
  - `struct Field { std::string name; FieldType type; }` : description d'un champ attendu pour un composant.
  - `using FieldValue = std::variant<math::Vector2f, float, std::string, int>` : valeur d'un champ parsé.
  - `using ComponentCreator = std::function<std::shared_ptr<ecs::IComponent>(const std::map<std::string, std::shared_ptr<FieldValue>>&)>`
  - `using ComponentAdder = std::function<void(std::shared_ptr<ecs::Registry>, ecs::Entity, std::shared_ptr<ecs::IComponent>)>`

Définitions de composants et champs attendus
Le fichier `ComponentParserCreators.cpp` renseigne les composants connus et leurs champs requis. Extrait :

- **TransformComponent** : `target`, `position` (VECTOR2F), `scale` (VECTOR2F), `rotation` (FLOAT)
- **VelocityComponent** : `target`
- **SpeedComponent** : `target`, `speed` (FLOAT)
- **SpriteComponent** : `target`, `filePath` (STRING)
- **AnimationComponent** : `target`, `animationPath` (STRING), `frameWidth` (FLOAT), `frameHeight` (FLOAT), `frameCount` (INT), `startWidth` (FLOAT), `startHeight` (FLOAT)
- **ControllableTag** : `target`
- **PlayerTag** : `target`
Overview
- **Purpose:** The parser converts entity files (prefabs) into `ParsedEntityPrefab` objects and registers them in the `EntityPrefabManager`.
- **Location:** `common/Parser`.

Key concepts
- **Main class:** `Parser`
  - **Constructor:** `Parser(std::shared_ptr<EntityPrefabManager> prefab, ParsingType type)` — `type` is `CLIENT` or `SERVER`.
  - **Important methods:**
    - `void parseAllEntities(std::string directoryPath)` — iterates a directory and calls `parseEntity` for each file.
    - `void parseEntity(std::string entityPath)` — parses a single entity file and registers the prefab in the `EntityPrefabManager`.
    - `std::shared_ptr<EntityPrefabManager> getPrefabManager() const` / `void setPrefabManager(...)` — access the prefab manager.
    - `bool isClientParsing() const` / `bool isServerParsing() const` — helpers for the `ParsingType`.
    - `bool shouldParseComponent(std::map<std::string, std::shared_ptr<FieldValue>> fields) const` — decides whether to parse a component based on the `target` field (`"both"`, `"client"`, `"server"`).

- **Types and utilities (in `ParserParam.hpp`):**
  - `enum class FieldType { VECTOR2F, FLOAT, STRING, INT }`
  - `struct Field { std::string name; FieldType type; }` — describes an expected field for a component.
  - `using FieldValue = std::variant<math::Vector2f, float, std::string, int>` — parsed field value.
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

Notes and possible extensions
- Components and their fields are hard-coded in `ComponentParserCreators.cpp`. To add a new component you need to:
  1. Add the definition (name, `type_index` and fields) in `instanciateComponentDefinitions()`;
  2. Add the `registerComponent<YourComponent>(...)` in `instanciateComponentCreators()` to provide the `ComponentCreator`.
- You could move component configuration to JSON/YAML to allow dynamic additions without recompiling.

Where to look in code
- Parser: `common/Parser/Parser.hpp`, `common/Parser/Parser.cpp`
- Params and types: `common/Parser/ParserParam.hpp`
- Definitions/creators: `common/Parser/ComponentParserCreators.cpp`

If you want, I can:
- add concrete examples (e.g. expected entity JSON),
- generate a simple flow diagram `file -> Parser -> ParsedEntityPrefab -> EntityPrefabManager`,
- or propose a PR to externalize component configuration.
