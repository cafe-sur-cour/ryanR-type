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
    CLIENT_LOCAL
};

struct EntityCreationContext {
    EntityCreationOrigin origin = EntityCreationOrigin::CLIENT_LOCAL;

    static EntityCreationContext forServer() {
        return {EntityCreationOrigin::SERVER};
    }

    static EntityCreationContext forLocalClient() {
        return {EntityCreationOrigin::CLIENT_LOCAL};
    }
};

} // namespace ecs

#endif /* !ENTITYCREATIONCONTEXT_HPP_ */
