#pragma once

#include "../../../common/gsm/IGameState.hpp"
#include "resourceManager/ResourceManager.hpp"

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
        std::shared_ptr<IGameStateMachine> _gsm;
        std::shared_ptr<ResourceManager> _resourceManager;
        std::vector<std::shared_ptr<ecs::ISystem>> _systems;
};

} // namespace gsm
