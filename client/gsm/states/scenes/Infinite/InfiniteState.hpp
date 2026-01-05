/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** InfiniteState
*/

#ifndef INFINITESTATE_HPP_
#define INFINITESTATE_HPP_

#include "../../base/AGameState.hpp"
#include <vector>
#include <string>
#include <memory>
#include "resourceManager/ResourceManager.hpp"
#include "../../../../../common/ECS/entity/registry/Registry.hpp"
#include "../../../../../common/Prefab/entityPrefabManager/EntityPrefabManager.hpp"
#include "../../../../../common/Parser/Parser.hpp"
#include "../../../../../common/interfaces/IWindow.hpp"

namespace gsm {

class InfiniteState : public AGameState {
    public:
        InfiniteState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ResourceManager> resourceManager);
        ~InfiniteState() override = default;

        void enter() override;
        void update(float deltaTime) override;
        void exit() override;

    private:
        std::shared_ptr<ecs::Registry> _registry;
        std::shared_ptr<EntityPrefabManager> _prefabManager;
        std::shared_ptr<Parser> _parser;
};

}  // namespace gsm

#endif  // INFINITESTATE_HPP_
