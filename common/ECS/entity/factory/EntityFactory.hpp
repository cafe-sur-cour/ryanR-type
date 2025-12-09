/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** EntityFactory
*/

#ifndef ENTITYFACTORY_HPP_
#define ENTITYFACTORY_HPP_

#include "IEntityFactory.hpp"
#include "../../../components/permanent/NetworkIdComponent.hpp"
#include <atomic>

namespace ecs {

class EntityFactory : public IEntityFactory {
    public:
        explicit EntityFactory(size_t startingNetworkId = 1)
            : _nextNetworkId(startingNetworkId) {}

        ~EntityFactory() = default;

        Entity createEntity(
            const std::shared_ptr<Registry>& registry,
            const EntityCreationContext& context = EntityCreationContext::forLocalClient()
        ) override {
            Entity entity = registry->createEntity();

            if (context.shouldHaveNetworkId()) {
                size_t networkId = resolveNetworkId(context);
                auto networkIdComponent = std::make_shared<NetworkIdComponent>(networkId);
                registry->addComponent<NetworkIdComponent>(entity, networkIdComponent);
            }

            return entity;
        }

        size_t getNextNetworkId() const override {
            return _nextNetworkId.load();
        }

        void setNextNetworkId(size_t nextId) override {
            _nextNetworkId.store(nextId);
        }

    private:
        size_t resolveNetworkId(const EntityCreationContext& context) {
            if (context.networkId.has_value()) {
                return context.networkId.value();
            }

            if (context.origin == EntityCreationOrigin::SERVER) {
                return _nextNetworkId.fetch_add(1);
            }

            return 0;
        }

        std::atomic<size_t> _nextNetworkId;
};

}  // namespace ecs

#endif /* !ENTITYFACTORY_HPP_ */
