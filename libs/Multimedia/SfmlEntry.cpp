/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SfmlEntry - Entry points for dynamic loading
*/

#include <memory>
#include "IWindow.hpp"
#include "SfmlWindow.hpp"
#include "SfmlEvent.hpp"
#include "SfmlAudio.hpp"
#include "../../common/ECS/resourceManager/ResourceManager.hpp"
#include "../../common/DLLoader/LoaderType.hpp"
#include "../../common/constants.hpp"

extern "C" {
    void* createWindow() {
        return new SfmlWindow("R-Type", constants::WINDOW_WIDTH, constants::WINDOW_HEIGHT);
    }

    void* createEvent(void* resourceManager, void* window) {
        auto resManager = static_cast<ecs::ResourceManager*>(resourceManager);
        auto rawWindow = static_cast<gfx::IWindow*>(window);
        auto sharedResManager = std::shared_ptr<ecs::ResourceManager>(
            resManager,
            [](ecs::ResourceManager*){}
        );
        auto sharedWindow = std::shared_ptr<gfx::IWindow>(
            rawWindow,
            [](gfx::IWindow*){}
        );
        return new SfmlEvent(sharedResManager, sharedWindow);
    }

    void* createAudio() {
        return new gfx::SfmlAudio();
    }

    int getType() {
        return MULTIMEDIA_MODULE;
    }
}
