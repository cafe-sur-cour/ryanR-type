/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** initRessourcesManager
*/

#include <iostream>
#include <memory>
#include <string>
#include "../../common/ECS/resourceManager/ResourceManager.hpp"
#include "../../common/DLLoader/DLLoader.hpp"
#include "../../common/DLLoader/LoaderType.hpp"
#include "../../libs/Multimedia/IEvent.hpp"
#include "../../libs/Multimedia/IWindow.hpp"
#include "initRessourcesManager.hpp"

std::shared_ptr<ecs::ResourceManager> initRessourcesManager(
    std::shared_ptr<DLLoader<gfx::createWindow_t>> windowLoader,
    std::shared_ptr<DLLoader<gfx::createEvent_t>> eventLoader
) {
    std::shared_ptr<ecs::ResourceManager> resourceManager =
        std::make_shared<ecs::ResourceManager>();

    gfx::createWindow_t createWindowFunc = windowLoader->getSymbol(
        "createWindow"
    );
    if (!createWindowFunc) {
        const char* error = windowLoader->Error();
        std::string errorMsg = "Failed to load createWindow from libMultimedia";
        if (error) {
            errorMsg += " - Error: ";
            errorMsg += error;
        }
        throw std::runtime_error(errorMsg);
    }
    std::shared_ptr<gfx::IWindow> window(createWindowFunc());

    gfx::createEvent_t createEventFunc = eventLoader->getSymbol("createEvent");
    if (!createEventFunc) {
        const char* error = eventLoader->Error();
        std::string errorMsg = "Failed to load createEvent from libMultimedia";
        if (error) {
            errorMsg += " - Error: ";
            errorMsg += error;
        }
        throw std::runtime_error(errorMsg);
    }
    std::shared_ptr<gfx::IEvent> event(
        createEventFunc(resourceManager.get(), window.get())
    );

    window->init();
    event->init();

    resourceManager->add<gfx::IWindow>(window);
    resourceManager->add<gfx::IEvent>(event);

    return resourceManager;
}
