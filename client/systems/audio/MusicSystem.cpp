/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** MusicSystem
*/

#include <memory>
#include <iostream>
#include "MusicSystem.hpp"
#include "../../components/temporary/MusicIntentComponent.hpp"
#include "../../components/rendering/MusicComponent.hpp"
#include "../../components/tags/BackGroundMusicTag.hpp"
#include "../../../common/ECS/view/View.hpp"
#include "../../../common/resourceManager/ResourceManager.hpp"
#include "../../../libs/Multimedia/IAudio.hpp"


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
            registry->removeComponent<MusicIntentComponent>(entityId);
        }
        return;
    }

    size_t musicEntityId = *musicIt;
    auto musicComp = registry->getComponent<MusicComponent>(musicEntityId);

    for (auto entityId : intentView) {
        auto intent = registry->getComponent<MusicIntentComponent>(entityId);

        if (!intent || !musicComp)
            continue;

        if (resourceManager->has<gfx::IAudio>()) {
            auto audio = resourceManager->get<gfx::IAudio>();
            switch (intent->getAction()) {
                case PLAY:
                    audio->setMusicVolume(intent->getVolume());
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
                    audio->setMusicVolume(intent->getVolume());
                    musicComp->playNewMusic(intent->getMusicPath());
                    break;
            }
        }

        registry->removeComponent<MusicIntentComponent>(entityId);
    }
}

}  // namespace ecs
