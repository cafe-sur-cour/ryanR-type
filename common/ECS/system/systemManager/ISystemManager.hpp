/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ISystemManager
*/

#ifndef ISystemMANAGER_HPP_
#define ISystemMANAGER_HPP_

#include "../ISystem.hpp"
#include <memory>

class ISystemManager {
    public:
        virtual ~ISystemManager();
        virtual void updateAllSystems(float deltaTime) = 0;
        virtual void addSystem(std::shared_ptr<ISystem> system) = 0;
        virtual void removeSystem(std::shared_ptr<ISystem> system) = 0;
};

#endif /* !ISystemMANAGER_HPP_ */
