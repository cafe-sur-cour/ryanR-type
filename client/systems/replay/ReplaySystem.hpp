/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ReplaySystem
*/

#ifndef REPLAYSYSTEM_HPP_
#define REPLAYSYSTEM_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <nlohmann/json.hpp>
#include "../../../common/systems/base/ASystem.hpp"
#include "../../../common/ECS/entity/registry/Registry.hpp"
#include "../../../common/resourceManager/ResourceManager.hpp"
#include "../components/rendering/SpriteComponent.hpp"
#include "../components/rendering/AnimationComponent.hpp"
#include "../components/rendering/ParallaxComponent.hpp"
#include "../components/rendering/HealthBarComponent.hpp"
#include "../components/rendering/TextComponent.hpp"
#include "../components/rendering/RectangleRenderComponent.hpp"
#include "../components/rendering/HitboxRenderComponent.hpp"
#include "../components/temporary/SoundIntentComponent.hpp"
#include "../../../common/components/permanent/GameZoneComponent.hpp"
#include "../../../common/components/permanent/ColliderComponent.hpp"
#include "../../../common/components/permanent/TransformComponent.hpp"
#include "../../../common/components/permanent/HealthComponent.hpp"

namespace ecs {

class ReplaySystem : public ASystem {
    public:
        ReplaySystem();
        ~ReplaySystem() = default;

        void update(std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<Registry> registry, float deltaTime) override;

    private:
        void saveReplayToFile(const nlohmann::json& frameData);

        std::string getSpriteId(const std::string& texturePath);
        std::filesystem::path getNextReplayFile();

        float _totalElapsedTime;
        std::filesystem::path _currentReplayFile;
};

} // namespace ecs

#endif /* !REPLAYSYSTEM_HPP_ */