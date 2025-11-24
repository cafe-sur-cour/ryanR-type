/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ASystemManager
*/

#ifndef ASystemMANAGER_HPP_
#define ASystemMANAGER_HPP_

#include <vector>
#include <memory>

#include "ISystemManager.hpp"
#include "../../context/AContext.hpp"
#include "../ISystem.hpp"

namespace ecs {

class ASystemManager : public ISystemManager {
    public:
        ASystemManager();
        ~ASystemManager();
        void updateAllSystems(std::shared_ptr<AContext> context, float deltaTime) override;
        void addSystem(std::shared_ptr<ISystem> system) override;
        void removeSystem(std::shared_ptr<ISystem> system) override;

    private:
        std::vector<std::shared_ptr<ISystem>> _systems;
};

} // namespace ecs

#endif /* !ASystemMANAGER_HPP_ */
