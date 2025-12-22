/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** EntityFactory
*/

#ifndef ENTITYFACTORY_HPP_
#define ENTITYFACTORY_HPP_

#include "IEntityFactory.hpp"
#include <atomic>

namespace ecs {

class EntityFactory : public IEntityFactory {
    public:
        explicit EntityFactory();
        ~EntityFactory() override;

        Entity createEntity(
            const std::shared_ptr<Registry>& registry,
            const EntityCreationContext& context = EntityCreationContext::forLocalClient()
        ) override;

    private:
        std::atomic<size_t> _nextLocalId;
};

}  // namespace ecs

#endif /* !ENTITYFACTORY_HPP_ */
