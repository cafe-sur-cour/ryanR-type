/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SoundSystem
*/

#include <memory>
#include "SoundSystem.hpp"
#include "../../../common/components/temporary/SoundIntentComponent.hpp"
#include "../../../common/ECS/view/View.hpp"
#include "../../../common/ECS/resourceManager/ResourceManager.hpp"
#include "../../../libs/Multimedia/IAudio.hpp"


namespace ecs {

SoundSystem::SoundSystem() {
}

void SoundSystem::update(std::shared_ptr<ResourceManager>
    resourceManager, std::shared_ptr<Registry> registry, float deltaTime) {
    (void)deltaTime;

    View<SoundIntentComponent> view(registry);

    for (auto entityId : view) {
        auto soundIntent = registry->getComponent<SoundIntentComponent>(entityId);

        if (!soundIntent)
            continue;

        if (resourceManager->has<gfx::IAudio>()) {
            auto audio = resourceManager->get<gfx::IAudio>();
            audio->playSound(soundIntent->getSoundPath(), soundIntent->getVolume());
        }

        soundIntent->setState(ComponentState::Processed);
    }
}

}  // namespace ecs
