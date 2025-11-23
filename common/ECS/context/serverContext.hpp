/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ServerContext
*/

#ifndef SERVERCONTEXT_HPP_
#define SERVERCONTEXT_HPP_

#include "AContext.hpp"

namespace ecs {

class ServerContext : public AContext {
    public:
        ServerContext() = default;
        ~ServerContext() override = default;
};

} // namespace ecs

#endif /* !SERVERCONTEXT_HPP_ */
