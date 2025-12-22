/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** MusicSystem
*/

#include <memory>
#include <iostream>
#include <cmath>
#include "MusicSystem.hpp"
#include "../../components/temporary/MusicIntentComponent.hpp"
#include "../../components/rendering/MusicComponent.hpp"
#include "../../components/tags/BackGroundMusicTag.hpp"
#include "../../../common/ECS/view/View.hpp"
#include "../../../common/resourceManager/ResourceManager.hpp"
#include "../../../libs/Multimedia/IAudio.hpp"
#include "../../../common/constants.hpp"
#include "../../../common/systems/SystemNames.hpp"
#include "../../SettingsConfig.hpp"


namespace ecs {

MusicSystem::MusicSystem() {
}

void MusicSystem::update(std::shared_ptr<ResourceManager>
    resourceManager, std::shared_ptr<Registry> registry, float deltaTime) {
    (void)deltaTime;

    View<MusicIntentComponent> intentView(registry);
    View<BackGroundMusicTag, MusicComponent> musicView(registry);

    auto musicIt = musicView.begin();
    auto musicEnd = musicView.end();
    bool hasMusic = (musicIt != musicEnd);
    if (!hasMusic) {
        for (auto entityId : intentView) {
            registry->removeOneComponent<MusicIntentComponent>(entityId);
        }
        return;
    }

    size_t musicEntityId = *musicIt;
    auto musicComp = registry->getComponent<MusicComponent>(musicEntityId);

    if (
        resourceManager->has<gfx::IAudio>() &&
        resourceManager->has<SettingsConfig>() &&
        musicComp
    ) {
        auto audio = resourceManager->get<gfx::IAudio>();
        float settingsVolume = resourceManager->get<SettingsConfig>()->getMusicVolume();
        float baseVolume = musicComp->getVolume();
        float effectiveVolume = (baseVolume / 100.0f) * settingsVolume;
        if (std::abs(audio->getMusicVolume() - effectiveVolume) > constants::EPS) {
            audio->setMusicVolume(effectiveVolume);
        }
    }

    for (auto entityId : intentView) {
        auto intent = registry->getComponent<MusicIntentComponent>(entityId);

        if (!intent || !musicComp)
            continue;

        if (resourceManager->has<gfx::IAudio>()) {
            auto audio = resourceManager->get<gfx::IAudio>();
            float baseVolume = musicComp->getVolume();
            float settingsVolume = 100.0f;
            if (resourceManager->has<SettingsConfig>()) {
                settingsVolume = resourceManager->get<SettingsConfig>()->getMusicVolume();
            }
            float effectiveVolume = (baseVolume / 100.0f) * settingsVolume;
            switch (intent->getAction()) {
                case PLAY:
                    audio->setMusicVolume(effectiveVolume);
                    audio->playMusic(musicComp->getCurrentMusic(),
                        musicComp->isLooping());
                    musicComp->playMusic();
                    break;
                case PAUSE:
                    audio->pauseMusic();
                    musicComp->pauseMusic();
                    break;
                case CHANGE:
                    audio->playMusic(intent->getMusicPath(),
                        musicComp->isLooping());
                    audio->setMusicVolume(effectiveVolume);
                    musicComp->playNewMusic(intent->getMusicPath());
                    break;
            }
        }

        registry->removeOneComponent<MusicIntentComponent>(entityId);
    }
}

}  // namespace ecs

extern "C" ecs::ISystem* createSystem() {
    return new ecs::MusicSystem();
}

extern "C" const char* getSystemName() {
    return ecs::systems::MUSIC_SYSTEM.c_str();
}

extern "C" void destroySystem(ecs::ISystem* system) {
    delete system;
}
