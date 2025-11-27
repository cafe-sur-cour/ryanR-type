/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** MultimediaEntry - Entry points for dynamic loading
*/

#include <memory>
#include "IWindow.hpp"
#include "IEvent.hpp"
#include "SfmlWindow.hpp"
#include "SfmlEvent.hpp"
#include "../../common/ECS/resourceManager/ResourceManager.hpp"
#include "../../common/DLLoader/LoaderType.hpp"

extern "C" {
    void* createWindow(const char* title, size_t width, size_t height) {
        return new SfmlWindow(std::string(title), width, height);
    }

    void* createEvent(void* resourceManager, gfx::IWindow* window) {
        auto resManager = static_cast<ecs::ResourceManager*>(resourceManager);
        auto sharedResManager = std::shared_ptr<ecs::ResourceManager>(
            resManager,
            [](ecs::ResourceManager*){}
        );
        auto sharedWindow = std::shared_ptr<gfx::IWindow>(
            window,
            [](gfx::IWindow*){}
        );
        return new SfmlEvent(sharedResManager, sharedWindow);
    }

    int getType() {
        return MULTIMEDIA_MODULE;
    }
}
