/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** main.cpp
*/

#include "initRessourcesManager/initRessourcesManager.hpp"
#include "../common/ECS/resourceManager/ResourceManager.hpp"

int main() {
    std::shared_ptr<ecs::ResourceManager> resourceManager = initRessourcesManager();
    return 0;
}
