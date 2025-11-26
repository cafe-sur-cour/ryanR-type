/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AssetManager
*/

#include "AssetManager.hpp"
#include "EmbeddedAssets.hpp"
#include <iostream>

namespace assets {

AssetManager::AssetManager() {
    /* Embedded assets are generated at compile time */
}

std::shared_ptr<AssetData> AssetManager::getAsset(const std::string& path) {
    auto it = embeddedAssets.find(path);
    if (it == embeddedAssets.end()) {
        std::cerr << "[AssetManager] Asset not found: " << path << std::endl;
        return nullptr;
    }
    return std::make_shared<AssetData>(it->second);
}

}  // namespace assets
