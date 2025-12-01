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
#include "../../ECS/resourceManager/ResourceManager.hpp"
#include "../../ECS/entity/registry/Registry.hpp"

namespace ecs {

class ASystem : public ISystem {
    public:
        ASystem();
        ~ASystem() = default;
        void updateSystem(std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<Registry> registry, float deltaTime) override;

    protected:
        virtual void update(std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<Registry> registry, float deltaTime) = 0;

    private:
};

} // namespace ecs

#endif /* !ASystem_HPP_ */
