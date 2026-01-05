/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** initResourcesManager
*/

#ifndef initResourcesManager_HPP_
#define initResourcesManager_HPP_

#include "../../common/resourceManager/ResourceManager.hpp"
#include "../Server.hpp"
#include "../Lobby.hpp"
#include "../../common/ECS/entity/registry/Registry.hpp"
#include "../../common/Parser/Parser.hpp"
#include "../../common/systems/systemManager/ISystemManager.hpp"
#include "../../common/InputMapping/IInputProvider.hpp"
#include "../gsm/machine/GameStateMachine.hpp"
#include <memory>

std::shared_ptr<ResourceManager> initResourcesManager(
    std::shared_ptr<rserv::Server> server,
    std::shared_ptr<rserv::Lobby> lobby
);

#endif /* !initResourcesManager_HPP_ */
