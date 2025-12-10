/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** IEntityFactory
*/

#ifndef IENTITYFACTORY_HPP_
#define IENTITYFACTORY_HPP_

#include <memory>
#include <string>
#include "../Entity.hpp"
#include "../EntityCreationContext.hpp"
#include "../registry/Registry.hpp"

namespace ecs {

class IEntityFactory {
    public:
        virtual ~IEntityFactory() = default;

        virtual Entity createEntity(
            const std::shared_ptr<Registry>& registry,
            const EntityCreationContext& context = EntityCreationContext::forLocalClient()
        ) = 0;

        virtual size_t getNextNetworkId() const = 0;
        virtual void setNextNetworkId(size_t nextId) = 0;
};

} // namespace ecs

#endif /* !IENTITYFACTORY_HPP_ */
