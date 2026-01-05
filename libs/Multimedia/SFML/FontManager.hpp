/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** FontManager
*/

#ifndef FONTMANAGER_HPP_
#define FONTMANAGER_HPP_

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <memory>
#include <string>
#include "../AssetManager/AssetManager.hpp"

namespace gfx {

class FontManager {
    public:
        FontManager(std::shared_ptr<assets::AssetManager> assetManager);
        ~FontManager() = default;

        std::shared_ptr<sf::Font> getFont(const std::string& path);

    private:
        std::shared_ptr<assets::AssetManager> _assetManager;
        std::unordered_map<std::string, std::shared_ptr<sf::Font>> _fontCache;
        std::unordered_map<std::string, std::vector<unsigned char>> _fontDataCache;
};

} // namespace gfx

#endif /* !FONTMANAGER_HPP_ */
