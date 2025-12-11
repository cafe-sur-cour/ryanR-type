/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** EntityPrefabManager
*/

#include <iostream>
#include <memory>
#include <string>
#include "EntityPrefabManager.hpp"
#include "../../Error/ParserError.hpp"

EntityPrefabManager::EntityPrefabManager() {
    clearPrefabs();
    _entityFactory = std::make_shared<ecs::EntityFactory>();
    _onEntityCreated = nullptr;
}

EntityPrefabManager::~EntityPrefabManager() {
    clearPrefabs();
}

void EntityPrefabManager::registerPrefab
    (const std::string &name, const std::shared_ptr<IPrefab> &prefab) {
    if (prefab)
        _prefabs[name] = prefab;
}

std::shared_ptr<IPrefab> EntityPrefabManager::getPrefab
    (const std::string &name) const {
    auto it = _prefabs.find(name);
    if (it != _prefabs.end()) {
        return it->second;
    }
    return nullptr;
}

bool EntityPrefabManager::hasPrefab(const std::string &name) const {
    return _prefabs.find(name) != _prefabs.end();
}

void EntityPrefabManager::deletePrefab(const std::string &name) {
    _prefabs.erase(name);
}

void EntityPrefabManager::clearPrefabs() {
    _prefabs.clear();
}

ecs::Entity EntityPrefabManager::createEntityFromPrefab(
    const std::string &prefabName,
    const std::shared_ptr<ecs::Registry> &registry,
    const ecs::EntityCreationContext &context
) {
    std::shared_ptr<IPrefab> prefab = getPrefab(prefabName);

    if (prefab && registry && _entityFactory) {
        ecs::Entity entity = prefab->instantiate(registry, _entityFactory, context);
        if (_onEntityCreated) {
            _onEntityCreated(entity, prefabName);
        }
        return entity;
    }
    throw err::ParserError(std::string("Prefab '") + prefabName +
        "' not found, registry is null, or factory is null.", err::ParserError::UNKNOWN);
}

ecs::Entity EntityPrefabManager::createEntityFromPrefab(const std::string
    &prefabName, const std::shared_ptr<ecs::Registry> &registry) {
    std::cout << "Creating entity from prefab: " << prefabName << std::endl;
    return createEntityFromPrefab(prefabName, registry,
        ecs::EntityCreationContext::forLocalClient());
}

std::shared_ptr<ecs::IEntityFactory> EntityPrefabManager::getEntityFactory() const {
    return _entityFactory;
}

void EntityPrefabManager::setEntityFactory(std::shared_ptr<ecs::IEntityFactory> factory) {
    _entityFactory = factory;
}

void EntityPrefabManager::setOnEntityCreated(std::function<void(ecs::Entity, const std::string&)> callback) {
    _onEntityCreated = callback;
}
