/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** SystemLoader
*/

#include "SystemLoader.hpp"
#include <iostream>
#include <string>
#include <utility>
#include <memory>
#include "../DLLoader/LoaderType.hpp"
#include "../DLLoader/DLLoader.hpp"

namespace ecs {

SystemLoader::SystemLoader() {
}

SystemLoader::~SystemLoader() {
    _loaders.clear();
}

std::shared_ptr<ISystem> SystemLoader::loadSystem(const std::string& name) {
    std::string libName = std::string(pathSystems) + "/lib" + name + sharedLibExt;
    auto loader = std::make_unique<DLLoader<ISystem*>>();

    if (!loader->Open(libName.c_str())) {
        std::cerr << "Failed to load system library: " << libName << std::endl;
        return nullptr;
    }

    void* sym = loader->Symbol("createSystem");
    if (!sym) {
        std::cerr << "Failed to get createSystem symbol from " << libName << std::endl;
        return nullptr;
    }

    createSystem_t createSystem = reinterpret_cast<createSystem_t>(sym);
    ISystem* instance = createSystem();
    if (!instance) {
        std::cerr << "Failed to create system from " << libName << std::endl;
        return nullptr;
    }

    _loaders[name] = std::move(loader);
    return std::shared_ptr<ISystem>(instance);
}

}  // namespace ecs
