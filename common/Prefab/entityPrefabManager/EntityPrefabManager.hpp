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
#include <functional>
#include "../../ECS/entity/Entity.hpp"
#include "../../ECS/entity/EntityCreationContext.hpp"
#include "../../ECS/entity/factory/IEntityFactory.hpp"
#include "../../ECS/entity/factory/EntityFactory.hpp"
#include "../IPrefab.hpp"

class EntityPrefabManager
{
    public:
        EntityPrefabManager();
        ~EntityPrefabManager();

        void registerPrefab(const std::string &name, const std::shared_ptr<IPrefab> &prefab);
        std::shared_ptr<IPrefab> getPrefab(const std::string &name) const;

        ecs::Entity createEntityFromPrefab(
            const std::string &prefabName,
            const std::shared_ptr<ecs::Registry> &registry,
            const ecs::EntityCreationContext &context
        );

        ecs::Entity createEntityFromPrefab(
            const std::string &prefabName,
            const std::shared_ptr<ecs::Registry> &registry
        );

        bool hasPrefab(const std::string &name) const;
        void deletePrefab(const std::string &name);
        void clearPrefabs();

        std::shared_ptr<ecs::IEntityFactory> getEntityFactory() const;
        void setEntityFactory(std::shared_ptr<ecs::IEntityFactory> factory);

        void setOnEntityCreated(std::function<void(ecs::Entity, const std::string&)> callback);

    private:
        std::map<std::string, std::shared_ptr<IPrefab>> _prefabs;
        std::shared_ptr<ecs::IEntityFactory> _entityFactory;
        std::function<void(ecs::Entity, const std::string&)> _onEntityCreated;
};

#endif /* !ENTITYPREFABMANAGER_HPP_ */
