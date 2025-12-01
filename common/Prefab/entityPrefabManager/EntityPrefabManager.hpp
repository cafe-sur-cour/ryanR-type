/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** EntityPrefabManager
*/

#ifndef ENTITYPREFABMANAGER_HPP_
#define ENTITYPREFABMANAGER_HPP_

#include <vector>
#include <string>
#include <map>
#include <memory>
#include "../../ECS/entity/Entity.hpp"
#include "../IPrefab.hpp"

class EntityPrefabManager
{
    public:
        EntityPrefabManager();
        ~EntityPrefabManager();

        void registerPrefab(const std::string &name, const std::shared_ptr<IPrefab> &prefab);
        std::shared_ptr<IPrefab> getPrefab(const std::string &name) const;
        ecs::Entity createEntityFromPrefab(const std::string &prefabName, const std::shared_ptr<ecs::Registry> &registry);
        bool hasPrefab(const std::string &name) const;
        void deletePrefab(const std::string &name);
        void clearPrefabs();
    private:
        std::map<std::string, std::shared_ptr<IPrefab>> _prefabs;
};

#endif /* !ENTITYPREFABMANAGER_HPP_ */
