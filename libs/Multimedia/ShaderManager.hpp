/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ShaderManager
*/

#ifndef SHADERMANAGER_HPP_
#define SHADERMANAGER_HPP_

#include <memory>
#include <string>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <SFML/Graphics.hpp>
#include "AssetManager/AssetManager.hpp"

class ShaderManager {
    public:
        ShaderManager(std::shared_ptr<assets::AssetManager> assetManager);
        ~ShaderManager() = default;

        std::shared_ptr<sf::Shader> loadShader(const std::string& path);

        std::shared_ptr<sf::Shader> getShader(const std::string& name) const;
        bool hasShader(const std::string& name) const;

        void enableFilter(const std::string& name);
        void addFilter(const std::string& name);
        void removeFilter(const std::string& name);
        bool isFilterActive(const std::string& name) const;
        void disableAllFilters();
        const std::set<std::string>& getActiveFilters() const;

        std::vector<std::shared_ptr<sf::Shader>> getActiveShaders() const;
        std::shared_ptr<sf::Shader> getCombinedShader();

        void clearCache();

    private:
        std::shared_ptr<assets::AssetManager> _assetManager;
        std::unordered_map<std::string, std::shared_ptr<sf::Shader>> _shaderCache;
        std::unordered_set<std::string> _failedShaders;
        std::set<std::string> _activeFilters;
        std::shared_ptr<sf::Shader> _combinedShader;
        std::vector<sf::RenderTexture> _intermediateTextures;

        void rebuildCombinedShader();
};

#endif /* !SHADERMANAGER_HPP_ */
