/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** TextureManager
*/

#include "TextureManager.hpp"
#include <iostream>
#include <memory>
#include <string>
namespace gfx {

TextureManager::TextureManager(std::shared_ptr<assets::AssetManager> assetManager) : _assetManager(assetManager) {}

std::shared_ptr<sf::Texture> TextureManager::loadTexture
(const std::string& path) {
    if (_failedTextures.find(path) != _failedTextures.end())
        return nullptr;
    auto it = _textureCache.find(path);
    if (it != _textureCache.end())
        return it->second;

    std::string normalizedPath = path;
    if (normalizedPath.find("./assets/") == 0)
        normalizedPath = normalizedPath.substr(9);
    if (normalizedPath.find("assets/") == 0)
        normalizedPath = normalizedPath.substr(7);

    auto asset = _assetManager->getAsset(normalizedPath);
    if (!asset) {
        std::cerr << "[TextureManager] Failed to load asset: " <<
        normalizedPath << std::endl;
        _failedTextures.insert(path);
        return nullptr;
    }

    auto texture = std::make_shared<sf::Texture>();
    if (!texture->loadFromMemory(asset->data.data(), asset->size)) {
        std::cerr << "[TextureManager] Failed to load texture from memory: " <<
        path << std::endl;
        _failedTextures.insert(path);
        return nullptr;
    }
    _textureCache[path] = texture;
    return texture;
}

void TextureManager::clearCache() {
    _textureCache.clear();
    _failedTextures.clear();
}

}  // namespace gfx
