/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** initResourcesManager
*/

#ifndef initResourcesManager_HPP_
#define initResourcesManager_HPP_

#include "../../common/resourceManager/ResourceManager.hpp"
#include <memory>
#include "../../common/DLLoader/DLLoader.hpp"
#include "../../libs/Multimedia/IWindow.hpp"
#include "../../libs/Multimedia/IEvent.hpp"
#include "../../libs/Multimedia/IAudio.hpp"

std::shared_ptr<ResourceManager> initResourcesManager(
    std::shared_ptr<DLLoader<gfx::createWindow_t>>,
    std::shared_ptr<DLLoader<gfx::createEvent_t>>,
    std::shared_ptr<DLLoader<gfx::createAudio_t>>,
    std::shared_ptr<ClientNetwork>
);

#endif /* !initResourcesManager_HPP_ */
