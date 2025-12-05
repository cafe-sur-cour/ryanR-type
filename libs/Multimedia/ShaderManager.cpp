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

ShaderManager::ShaderManager(std::shared_ptr<assets::AssetManager> assetManager)
    : _assetManager(assetManager), _combinedShader(nullptr) {
}

std::shared_ptr<sf::Shader> ShaderManager::loadShader(const std::string& path) {
    if (_failedShaders.find(path) != _failedShaders.end())
        return nullptr;

    auto it = _shaderCache.find(path);
    if (it != _shaderCache.end())
        return it->second;

    std::string normalizedPath = path;
    if (normalizedPath.find("./assets/") == 0)
        normalizedPath = normalizedPath.substr(9);
    if (normalizedPath.find("assets/") == 0)
        normalizedPath = normalizedPath.substr(7);

    auto asset = _assetManager->getAsset(normalizedPath);
    if (!asset) {
        std::cerr << "[ShaderManager] Failed to load shader asset: " << path << std::endl;
        _failedShaders.insert(path);
        return nullptr;
    }

    auto shader = std::make_shared<sf::Shader>();

    if (!sf::Shader::isAvailable()) {
        std::cerr << "[ShaderManager] Shaders are not available on this system" << std::endl;
        _failedShaders.insert(path);
        return nullptr;
    }

    if (!shader->loadFromMemory(std::string(asset->data.begin(), asset->data.end()),
                                sf::Shader::Type::Fragment)) {
        std::cerr << "[ShaderManager] Failed to load fragment shader from memory: "
            << path << std::endl;
        _failedShaders.insert(path);
        return nullptr;
    }

    _shaderCache[path] = shader;
    return shader;
}

std::shared_ptr<sf::Shader> ShaderManager::getShader(const std::string& name) const {
    auto it = _shaderCache.find(name);
    if (it != _shaderCache.end()) {
        return it->second;
    }
    return nullptr;
}

bool ShaderManager::hasShader(const std::string& name) const {
    return _shaderCache.find(name) != _shaderCache.end();
}

void ShaderManager::enableFilter(const std::string& path) {
    if (!hasShader(path)) {
        loadShader(path);
    }

    _activeFilters.clear();
    _activeFilters.insert(path);
    rebuildCombinedShader();
}

void ShaderManager::addFilter(const std::string& path) {
    if (!hasShader(path)) {
        loadShader(path);
    }

    _activeFilters.insert(path);
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
            auto it = _uniforms.find(filterName);
            if (it != _uniforms.end()) {
                for (const auto& uniform : it->second) {
                    shader->setUniform(uniform.first, uniform.second);
                }
            }
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

void ShaderManager::clearCache() {
    _shaderCache.clear();
    _failedShaders.clear();
    _activeFilters.clear();
    _combinedShader = nullptr;
    _uniforms.clear();
}

void ShaderManager::setUniform(
    const std::string& shaderName,
    const std::string& uniformName,
    float value
) {
    _uniforms[shaderName][uniformName] = value;

    if (_combinedShader) {
        rebuildCombinedShader();
    }
}

void ShaderManager::clearUniforms(const std::string& shaderName) {
    _uniforms.erase(shaderName);
    if (_combinedShader) {
        rebuildCombinedShader();
    }
}
