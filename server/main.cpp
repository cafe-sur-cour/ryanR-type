/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Main
*/

#include <memory>
#include "initRessourcesManager/initRessourcesManager.hpp"
#include "../common/ECS/resourceManager/ResourceManager.hpp"

int main() {
    std::shared_ptr<ecs::ResourceManager> resourceManager =
        initRessourcesManager();
    return 0;
}
