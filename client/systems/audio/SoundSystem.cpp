/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SoundSystem
*/

#include <memory>
#include <cmath>
#include "SoundSystem.hpp"
#include "../../components/temporary/SoundIntentComponent.hpp"
#include "../../../common/ECS/view/View.hpp"
#include "../../../common/resourceManager/ResourceManager.hpp"
#include "../../../libs/Multimedia/IAudio.hpp"
#include "../../../common/constants.hpp"
#include "../../../common/systems/SystemNames.hpp"
#include "../../SettingsConfig.hpp"


namespace ecs {

SoundSystem::SoundSystem() {
}

void SoundSystem::update(std::shared_ptr<ResourceManager>
    resourceManager, std::shared_ptr<Registry> registry, float deltaTime) {
    (void)deltaTime;

    if (resourceManager->has<gfx::IAudio>() && resourceManager->has<SettingsConfig>()) {
        auto audio = resourceManager->get<gfx::IAudio>();
        float settingsVol = resourceManager->get<SettingsConfig>()->getSoundVolume();
        if (std::abs(audio->getSoundVolume() - settingsVol) > constants::EPS) {
            audio->setSoundVolume(settingsVol);
        }
    }

    View<SoundIntentComponent> view(registry);

    for (auto entityId : view) {
        auto soundIntent = registry->getComponent<SoundIntentComponent>(entityId);

        if (!soundIntent)
            continue;

        if (resourceManager->has<gfx::IAudio>()) {
            auto audio = resourceManager->get<gfx::IAudio>();
            float volume = soundIntent->getVolume();
            if (resourceManager->has<SettingsConfig>()) {
                float settingsVol = resourceManager->get<SettingsConfig>()->getSoundVolume();
                volume = (volume / 100.0f) * settingsVol;
            }
            audio->playSound(soundIntent->getSoundPath(), volume);
        }

        registry->removeOneComponent<SoundIntentComponent>(entityId);
    }
}

}  // namespace ecs

extern "C" ecs::ISystem* createSystem() {
    return new ecs::SoundSystem();
}

extern "C" const char* getSystemName() {
    return ecs::systems::SOUND_SYSTEM.c_str();
}

extern "C" void destroySystem(ecs::ISystem* system) {
    delete system;
}
