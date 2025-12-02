/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** initRessourcesManager
*/

#ifndef INITRESSOURCESMANAGER_HPP_
#define INITRESSOURCESMANAGER_HPP_

#include "../../common/ECS/resourceManager/ResourceManager.hpp"
#include <memory>
#include "../../common/DLLoader/DLLoader.hpp"
#include "../../libs/Multimedia/IWindow.hpp"
#include "../../libs/Multimedia/IEvent.hpp"
#include "../../libs/Multimedia/IAudio.hpp"

std::shared_ptr<ecs::ResourceManager> initRessourcesManager(
    std::shared_ptr<DLLoader<gfx::createWindow_t>>,
    std::shared_ptr<DLLoader<gfx::createEvent_t>>,
    std::shared_ptr<DLLoader<gfx::createAudio_t>>
);

#endif /* !INITRESSOURCESMANAGER_HPP_ */
