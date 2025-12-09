/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** EntityCreationContext
*/

#ifndef ENTITYCREATIONCONTEXT_HPP_
#define ENTITYCREATIONCONTEXT_HPP_

#include <cstddef>
#include <optional>

namespace ecs {

enum class EntityCreationOrigin {
    SERVER,
    CLIENT_FROM_NETWORK,
    CLIENT_LOCAL
};

struct EntityCreationContext {
    EntityCreationOrigin origin = EntityCreationOrigin::CLIENT_LOCAL;
    std::optional<size_t> networkId = std::nullopt;

    static EntityCreationContext forServer(std::optional<size_t> existingId = std::nullopt) {
        return {EntityCreationOrigin::SERVER, existingId};
    }

    static EntityCreationContext forNetworkSync(size_t serverNetworkId) {
        return {EntityCreationOrigin::CLIENT_FROM_NETWORK, serverNetworkId};
    }

    static EntityCreationContext forLocalClient() {
        return {EntityCreationOrigin::CLIENT_LOCAL, std::nullopt};
    }

    bool shouldHaveNetworkId() const {
        return origin == EntityCreationOrigin::SERVER ||
               origin == EntityCreationOrigin::CLIENT_FROM_NETWORK;
    }
};

} // namespace ecs

#endif /* !ENTITYCREATIONCONTEXT_HPP_ */
