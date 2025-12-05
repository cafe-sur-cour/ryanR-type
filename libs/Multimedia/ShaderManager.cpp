/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ShaderManager
*/

#include "ShaderManager.hpp"
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <memory>

ShaderManager::ShaderManager() : _combinedShader(nullptr) {
}

bool ShaderManager::loadShader(const std::string& name, const std::string& vertexPath,
                               const std::string& fragmentPath) {
    auto shader = std::make_shared<sf::Shader>();

    bool loaded = false;

    if (!sf::Shader::isAvailable()) {
        std::cerr << "Shaders are not available on this system" << std::endl;
        return false;
    }

    if (vertexPath.empty()) {
        loaded = shader->loadFromFile(fragmentPath, sf::Shader::Type::Fragment);
        if (!loaded) {
            std::cerr << "Failed to load fragment shader '" << name << "' from: "
                      << fragmentPath << std::endl;
        }
    } else {
        loaded = shader->loadFromFile(vertexPath, fragmentPath);
        if (!loaded) {
            std::cerr << "Failed to load shader '" << name << "' from: "
                      << vertexPath << " / " << fragmentPath << std::endl;
        }
    }

    if (loaded) {
        _shaders[name] = shader;
        return true;
    }
    return false;
}

std::shared_ptr<sf::Shader> ShaderManager::getShader(const std::string& name) const {
    auto it = _shaders.find(name);
    if (it != _shaders.end()) {
        return it->second;
    }
    return nullptr;
}

bool ShaderManager::hasShader(const std::string& name) const {
    return _shaders.find(name) != _shaders.end();
}

void ShaderManager::enableFilter(const std::string& name) {
    if (!hasShader(name)) {
        std::cerr << "Filter not found: " << name << std::endl;
        return;
    }

    _activeFilters.clear();
    _activeFilters.insert(name);
    rebuildCombinedShader();
}

void ShaderManager::addFilter(const std::string& name) {
    if (!hasShader(name)) {
        std::cerr << "Filter not found: " << name << std::endl;
        return;
    }

    _activeFilters.insert(name);
    rebuildCombinedShader();
}

void ShaderManager::removeFilter(const std::string& name) {
    _activeFilters.erase(name);
    rebuildCombinedShader();
}

bool ShaderManager::isFilterActive(const std::string& name) const {
    return _activeFilters.find(name) != _activeFilters.end();
}

void ShaderManager::disableAllFilters() {
    _activeFilters.clear();
    _combinedShader = nullptr;
}

const std::set<std::string>& ShaderManager::getActiveFilters() const {
    return _activeFilters;
}

std::vector<std::shared_ptr<sf::Shader>> ShaderManager::getActiveShaders() const {
    std::vector<std::shared_ptr<sf::Shader>> shaders;
    for (const auto& filterName : _activeFilters) {
        auto shader = getShader(filterName);
        if (shader) {
            shaders.push_back(shader);
        }
    }
    return shaders;
}

void ShaderManager::rebuildCombinedShader() {
    if (_activeFilters.empty()) {
        _combinedShader = nullptr;
        return;
    }

    if (_activeFilters.size() == 1) {
        _combinedShader = getShader(*_activeFilters.begin());
        return;
    }

    _combinedShader = getShader(*_activeFilters.begin());
}

std::shared_ptr<sf::Shader> ShaderManager::getCombinedShader() {
    return _combinedShader;
}
