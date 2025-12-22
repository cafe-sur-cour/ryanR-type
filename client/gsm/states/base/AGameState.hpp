#pragma once

#include "../../../../common/gsm/IGameState.hpp"
#include "../../../../common/resourceManager/ResourceManager.hpp"
#include "../../../../common/systems/SystemLoader.hpp"

namespace gsm {

class AGameState : public IGameState {
    public:
        AGameState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ResourceManager> resourceManager);
        ~AGameState() override = default;

        void enter() override;
        void update(float deltaTime) override;
        void exit() override;
        std::vector<std::shared_ptr<ecs::ISystem>> getSystems() const override;

    protected:
        void addSystem(std::shared_ptr<ecs::ISystem> system) override;
        void addSystem(const std::string& systemName) override;
        std::weak_ptr<IGameStateMachine> _gsm;
        std::shared_ptr<ResourceManager> _resourceManager;
        std::shared_ptr<ecs::SystemLoader> _systemLoader;
        std::vector<std::shared_ptr<ecs::ISystem>> _systems;
};

} // namespace gsm
