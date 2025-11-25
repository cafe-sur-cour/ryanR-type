/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ISystem
*/

#ifndef ISystem_HPP_
#define ISystem_HPP_

#include "../../resourceManager/ResourceManager.hpp"
#include "../../entity/registry/ARegistry.hpp"
#include <memory>

namespace ecs {

class ISystem {
    public:
        virtual ~ISystem() = default;
        virtual void updateSystem(std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<ARegistry> registry, float deltaTime) = 0;
};

} // namespace ecs

#endif /* !ISystem_HPP_ */
