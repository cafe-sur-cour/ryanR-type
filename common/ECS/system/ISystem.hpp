/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ISystem
*/

#ifndef ISystem_HPP_
#define ISystem_HPP_

#include "../context/AContext.hpp"
#include <memory>

namespace ecs {

class ISystem {
    public:
        virtual ~ISystem();
        virtual void updateSystem(std::shared_ptr<AContext> context, float deltaTime) = 0;
};

} // namespace ecs

#endif /* !ISystem_HPP_ */
