/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AssetManager
*/

#include <iostream>
#include <memory>
#include <string>
#include "AssetManager.hpp"
#include "EmbeddedAssets.hpp"

namespace assets {

AssetManager::AssetManager() {
    /* Embedded assets are generated at compile time */
}

std::shared_ptr<AssetData> AssetManager::getAsset(const std::string& path) {
    auto& assets = getEmbeddedAssets();
    auto it = assets.find(path);
    if (it == assets.end()) {
        std::cerr << "[AssetManager] Asset not found: " << path << std::endl;
        return nullptr;
    }
    return std::make_shared<AssetData>(it->second);
}

}  // namespace assets
