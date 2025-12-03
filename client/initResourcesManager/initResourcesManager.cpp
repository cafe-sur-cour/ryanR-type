/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** initResourcesManager
*/

#include <iostream>
#include <memory>
#include <string>
#include "../../common/resourceManager/ResourceManager.hpp"
#include "GraphicalInputProvider.hpp"
#include "../ClientNetwork.hpp"
#include "../../common/DLLoader/DLLoader.hpp"
#include "../../common/DLLoader/LoaderType.hpp"
#include "../../libs/Multimedia/IEvent.hpp"
#include "../../libs/Multimedia/IWindow.hpp"
#include "../../libs/Multimedia/IAudio.hpp"
#include "../../common/InputMapping/InputMappingManager.hpp"
#include "initResourcesManager.hpp"

std::shared_ptr<ResourceManager> initResourcesManager(
    std::shared_ptr<DLLoader<gfx::createWindow_t>> windowLoader,
    std::shared_ptr<DLLoader<gfx::createEvent_t>> eventLoader,
    std::shared_ptr<DLLoader<gfx::createAudio_t>> audioLoader,
    std::shared_ptr<ClientNetwork> clientNetwork
) {
    std::shared_ptr<ResourceManager> resourceManager =
        std::make_shared<ResourceManager>();

    gfx::createWindow_t createWindowFunc = windowLoader->getSymbol(
        "createWindow"
    );
    if (!createWindowFunc) {
        std::string error = windowLoader->Error();
        std::string errorMsg = "Failed to load createWindow from libMultimedia";
        if (!error.empty()) {
            errorMsg += " - Error: " + error;
        }
        throw std::runtime_error(errorMsg);
    }
    std::shared_ptr<gfx::IWindow> window(createWindowFunc());

    gfx::createEvent_t createEventFunc = eventLoader->getSymbol("createEvent");
    if (!createEventFunc) {
        std::string error = eventLoader->Error();
        std::string errorMsg = "Failed to load createEvent from libMultimedia";
        if (!error.empty()) {
            errorMsg += " - Error: " + error;
        }
        throw std::runtime_error(errorMsg);
    }
    std::shared_ptr<gfx::IEvent> event(
        createEventFunc(resourceManager.get(), window.get())
    );

    gfx::createAudio_t createAudioFunc = audioLoader->getSymbol("createAudio");
    if (!createAudioFunc) {
        std::string error = audioLoader->Error();
        std::string errorMsg = "Failed to load createAudio from libMultimedia";
        if (!error.empty()) {
            errorMsg += " - Error: " + error;
        }
        throw std::runtime_error(errorMsg);
    }
    std::shared_ptr<gfx::IAudio> audio(createAudioFunc());

    window->init();
    event->init();

    resourceManager->add<gfx::IWindow>(window);
    resourceManager->add<gfx::IEvent>(event);
    resourceManager->add<gfx::IAudio>(audio);
    resourceManager->add<ClientNetwork>(clientNetwork);

    auto mappingManager = std::make_shared<ecs::InputMappingManager>();
    mappingManager->loadDefault();
    auto inputProvider = std::make_shared<ecs::GraphicalInputProvider>(
        event,
        mappingManager->getMapping());
    resourceManager->add<ecs::IInputProvider>(inputProvider);

    return resourceManager;
}
