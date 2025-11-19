/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ISystemManager
*/

#ifndef ISystemMANAGER_HPP_
#define ISystemMANAGER_HPP_

#include "../ISystem.hpp"
#include "../context/AContext.hpp"
#include <memory>

class ISystemManager {
    public:
        virtual ~ISystemManager();
        virtual void updateAllSystems(std::shared_ptr<AContext> context, float deltaTime) = 0;
        virtual void addSystem(std::shared_ptr<ISystem> system) = 0;
        virtual void removeSystem(std::shared_ptr<ISystem> system) = 0;
};

#endif /* !ISystemMANAGER_HPP_ */
