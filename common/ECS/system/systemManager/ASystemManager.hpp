/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ASystemManager
*/

#ifndef ASystemMANAGER_HPP_
#define ASystemMANAGER_HPP_

#include "ISystemManager.hpp"
#include "../ISystem.hpp"
#include <vector>
#include <memory>

class ASystemManager : public ISystemManager {
    public:
        ASystemManager();
        ~ASystemManager();
        void updateAllSystems(float deltaTime) override; // window en plus a ajouter
        void addSystem(std::shared_ptr<ISystem> system) override;
        void removeSystem(std::shared_ptr<ISystem> system) override;

    private:
        std::vector<std::shared_ptr<ISystem>> _systems;
};

#endif /* !ASystemMANAGER_HPP_ */
