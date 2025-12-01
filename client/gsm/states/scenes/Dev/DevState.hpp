#ifndef DEVSTATE_HPP_
#define DEVSTATE_HPP_

#include "../../base/AGameState.hpp"
#include "../../../../../common/ECS/resourceManager/ResourceManager.hpp"
#include "../../../../../common/ECS/entity/registry/Registry.hpp"
#include "../../../../../common/systems/systemManager/ASystemManager.hpp"
#include "../../../../../common/systems/movement/MovementSystem.hpp"
#include "../../../../../common/systems/movement/InputToVelocitySystem.hpp"
#include "../../../../systems/input/MovementInputSystem.hpp"
#include "../../../../systems/input/CoinDropSystem.hpp"
#include "../../../../systems/rendering/SpriteRenderingSystem.hpp"
#include "../../../../systems/audio/SoundSystem.hpp"
#include "../../../../../common/Prefab/entityPrefabManager/EntityPrefabManager.hpp"
namespace gsm {

class DevState : public AGameState {
public:
    DevState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ecs::ResourceManager> resourceManager);
    ~DevState() override = default;

    void enter() override;
    void update(float deltaTime) override;
    void exit() override;

private:
    std::shared_ptr<ecs::ResourceManager> _resourceManager;
    std::shared_ptr<ecs::Registry> _registry;
    std::shared_ptr<ecs::ASystemManager> _systemManager;
    std::shared_ptr<ecs::MovementSystem> _movementSystem;
    std::shared_ptr<ecs::InputToVelocitySystem> _inputToVelocitySystem;
    std::shared_ptr<ecs::MovementInputSystem> _inputSystem;
    std::shared_ptr<ecs::CoinDropSystem> _coinDropSystem;
    std::shared_ptr<ecs::SpriteRenderingSystem> _spriteRenderingSystem;
    std::shared_ptr<ecs::SoundSystem> _soundSystem;
    std::shared_ptr<EntityPrefabManager> _prefabManager;
    ecs::Entity _playerId;
};

}  // namespace gsm

#endif  // DEVSTATE_HPP_
