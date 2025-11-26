/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** TextureManager
*/

#ifndef TEXTUREMANAGER_HPP_
#define TEXTUREMANAGER_HPP_

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <string>

namespace gfx {

class TextureManager {
    public:
        TextureManager() = default;
        ~TextureManager() = default;

        std::shared_ptr<sf::Texture> loadTexture(const std::string& path);

        void clearCache();
        size_t getCacheSize() const { return _textureCache.size(); }

    private:
        std::unordered_map<std::string, std::shared_ptr<sf::Texture>> _textureCache;
        std::unordered_set<std::string> _failedTextures;
};

} // namespace gfx

#endif /* !TEXTUREMANAGER_HPP_ */
