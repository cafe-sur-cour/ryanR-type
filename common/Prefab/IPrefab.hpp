/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** IPrefab
*/

#ifndef IPREFAB_HPP_
#define IPREFAB_HPP_
#include <memory>
#include "../ECS/entity/registry/Registry.hpp"
#include "../ECS/entity/Entity.hpp"
#include "../ECS/entity/EntityCreationContext.hpp"
#include "../ECS/entity/factory/IEntityFactory.hpp"

class IPrefab {
    public:
        virtual ~IPrefab() = default;

        virtual ecs::Entity instantiate(
            const std::shared_ptr<ecs::Registry>& registry,
            const std::shared_ptr<ecs::IEntityFactory>& factory,
            const ecs::EntityCreationContext& context = ecs::EntityCreationContext::forLocalClient()
        ) = 0;

        virtual ecs::Entity instantiate(const std::shared_ptr<ecs::Registry>& registry) = 0;
};

#endif /* !IPREFAB_HPP_ */
