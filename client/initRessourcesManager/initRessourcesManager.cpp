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

std::shared_ptr<ecs::ResourceManager> initRessourcesManager() {
    std::shared_ptr<ecs::ResourceManager> resourceManager =
        std::make_shared<ecs::ResourceManager>();

    std::string multimediaPath = std::string(pathLoad) + "/" + multimediaLib;
    DLLoader<createWindow_t> multimediaLoaderWindow;
    void* handle = multimediaLoaderWindow.Open(multimediaPath.c_str());
    if (!handle) {
        const char* error = multimediaLoaderWindow.Error();
        std::string errorMsg = "Failed to load Multimedia library: ";
        errorMsg += multimediaPath;
        if (error) {
            errorMsg += " - Error: ";
            errorMsg += error;
        }
        throw std::runtime_error(errorMsg);
    }

    createWindow_t createWindowFunc =
        multimediaLoaderWindow.getSymbol("createWindow");
    if (!createWindowFunc) {
        const char* error = multimediaLoaderWindow.Error();
        std::string errorMsg = "Failed to load createWindow from libMultimedia";
        if (error) {
            errorMsg += " - Error: ";
            errorMsg += error;
        }
        throw std::runtime_error(errorMsg);
    }
    gfx::IWindow* rawWindow = static_cast<gfx::IWindow*>(createWindowFunc());
    std::shared_ptr<gfx::IWindow> window(rawWindow);

    DLLoader<createEvent_t> multimediaLoaderEvent;
    multimediaLoaderEvent.Open(multimediaPath.c_str());

    createEvent_t createEventFunc =
        multimediaLoaderEvent.getSymbol("createEvent");
    if (!createEventFunc) {
        const char* error = multimediaLoaderWindow.Error();
        std::string errorMsg = "Failed to load createEvent from libMultimedia";
        if (error) {
            errorMsg += " - Error: ";
            errorMsg += error;
        }
        throw std::runtime_error(errorMsg);
    }
    gfx::IEvent* rawEvent = static_cast<gfx::IEvent*>(
        createEventFunc(resourceManager.get(), window.get()));
    std::shared_ptr<gfx::IEvent> event(rawEvent);

    window->init();
    event->init();

    resourceManager->add<gfx::IWindow>(window);
    resourceManager->add<gfx::IEvent>(event);

    return resourceManager;
}
