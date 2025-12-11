/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** DevState
*/

#ifndef DEVSTATE_HPP_
#define DEVSTATE_HPP_

#include "../../base/AGameState.hpp"
#include "resourceManager/ResourceManager.hpp"
#include "../../../../../common/ECS/entity/registry/Registry.hpp"
#include "../../../../../common/Prefab/entityPrefabManager/EntityPrefabManager.hpp"
#include "../../../../../common/Parser/Parser.hpp"

namespace gsm {

class DevState : public AGameState {
    public:
        DevState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ResourceManager> resourceManager);
        ~DevState() override = default;

        void enter() override;
        void update(float deltaTime) override;
        void exit() override;

    private:
        void renderHUD();

    private:
        std::shared_ptr<ecs::Registry> _registry;
        std::shared_ptr<EntityPrefabManager> _prefabManager;
        std::shared_ptr<Parser> _parser;
};

}  // namespace gsm

#endif  // DEVSTATE_HPP_

