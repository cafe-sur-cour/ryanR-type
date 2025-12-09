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
        explicit EntityFactory(size_t startingNetworkId = 1);
        ~EntityFactory() override;

        Entity createEntity(
            const std::shared_ptr<Registry>& registry,
            const EntityCreationContext& context = EntityCreationContext::forLocalClient()
        ) override;

        size_t getNextNetworkId() const override;
        void setNextNetworkId(size_t nextId) override;

    private:
        size_t resolveNetworkId(const EntityCreationContext& context);
        std::atomic<size_t> _nextNetworkId;
};

}  // namespace ecs

#endif /* !ENTITYFACTORY_HPP_ */
