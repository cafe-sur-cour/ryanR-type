/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** EntityPrefabManager
*/

#include "EntityPrefabManager.hpp"
#include <iostream>
#include <memory>
#include <string>

EntityPrefabManager::EntityPrefabManager() {
    clearPrefabs();
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

size_t EntityPrefabManager::createEntityFromPrefab(const std::string
    &prefabName, const std::shared_ptr<ecs::ARegistry> &registry) {
    std::shared_ptr<IPrefab> prefab = getPrefab(prefabName);

    if (prefab && registry)
        return prefab->instantiate(registry);
    std::cout << "Error: Prefab '" << prefabName <<
        "' not found or registry is null." << std::endl;
    return 0;
}
