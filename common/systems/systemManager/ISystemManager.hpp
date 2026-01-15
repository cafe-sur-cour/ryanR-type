/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ISystemManager
*/

#ifndef ISystemMANAGER_HPP_
#define ISystemMANAGER_HPP_

#include <memory>

#include "../base/ISystem.hpp"
#include "../../resourceManager/ResourceManager.hpp"
#include "../../ECS/entity/registry/Registry.hpp"

namespace ecs {

class ISystemManager {
    public:
        virtual ~ISystemManager() = default;
        virtual void updateAllSystems(std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<Registry> registry, float deltaTime) = 0;
        virtual void addSystem(std::shared_ptr<ISystem> system) = 0;
        virtual void removeSystem(std::shared_ptr<ISystem> system) = 0;
        virtual void clearAllSystems() = 0;
};

}  // namespace ecs

#endif /* !ISystemMANAGER_HPP_ */
