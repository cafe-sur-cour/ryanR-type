/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ASystem
*/

#ifndef ASystem_HPP_
#define ASystem_HPP_

#include <memory>

#include "ISystem.hpp"
#include "../resourceManager/ResourceManager.hpp"
#include "../entity/registry/ARegistry.hpp"

namespace ecs {

class ASystem : public ISystem {
    public:
        ASystem();
        ~ASystem() = default;
        void updateSystem(std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<ARegistry> registry, float deltaTime) override;

    protected:
        virtual void update(std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<ARegistry> registry, float deltaTime) = 0;

    private:
};

} // namespace ecs

#endif /* !ASystem_HPP_ */
