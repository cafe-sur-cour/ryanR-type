/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LevelEditorUtils
*/

#include <filesystem>  // NOLINT(build/c++17)
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "LevelEditorState.hpp"
#include "../../../../constants.hpp"

namespace gsm {

bool LevelEditorState::validateFields() {
    if (_levelNameInput && _levelNameInput->getText().empty()) {
        return false;
    }
    if (_mapLengthInput && _mapLengthInput->getText().empty()) {
        return false;
    }
    if (_scrollSpeedInput && _scrollSpeedInput->getText().empty()) {
        return false;
    }
    return true;
}

void LevelEditorState::updateSaveButtonText() {
    if (_saveButton) {
        std::string buttonText = "Save Level";
        if (_hasUnsavedChanges) {
            buttonText = "Save Level *";
        }
        _saveButton->setText(buttonText);
    }
}

std::vector<std::string> LevelEditorState::loadAvailableMusics() {
    std::vector<std::string> musics;
    std::filesystem::path musicsPath = constants::MUSIC_DIRECTORY;

    if (!std::filesystem::exists(musicsPath) || !std::filesystem::is_directory(musicsPath)) {
        return musics;
    }

    for (const auto& entry : std::filesystem::directory_iterator(musicsPath)) {
        if (entry.is_regular_file() && entry.path().extension() ==
            constants::LEVEL_FILE_EXTENSION) {
            try {
                std::ifstream file(entry.path());
                nlohmann::json musicData;
                file >> musicData;
                file.close();

                if (musicData.contains(constants::NAME_FIELD) &&
                    musicData[constants::NAME_FIELD].is_string()) {
                    std::string musicName = musicData[constants::NAME_FIELD];
                    musics.push_back(musicName);
                }
            } catch (const std::exception&) {
            }
        }
    }

    return musics;
}

std::vector<std::string> LevelEditorState::loadAvailableBackgrounds() {
    std::vector<std::string> backgrounds;
    std::filesystem::path backgroundsPath = constants::BACKGROUNDS_DIRECTORY;

    if (!std::filesystem::exists(backgroundsPath) ||
        !std::filesystem::is_directory(backgroundsPath)) {
        return backgrounds;
    }

    for (const auto& entry : std::filesystem::directory_iterator(backgroundsPath)) {
        if (entry.is_regular_file() && entry.path().extension() ==
            constants::LEVEL_FILE_EXTENSION) {
            try {
                std::ifstream file(entry.path());
                nlohmann::json backgroundData;
                file >> backgroundData;
                file.close();

                if (backgroundData.contains(constants::NAME_FIELD) &&
                    backgroundData[constants::NAME_FIELD].is_string()) {
                    std::string backgroundName = backgroundData[constants::NAME_FIELD];
                    backgrounds.push_back(backgroundName);
                }
            } catch (const std::exception&) {
            }
        }
    }

    return backgrounds;
}

std::vector<std::string> LevelEditorState::loadAvailableObstacles() {
    std::vector<std::string> obstacles;
    std::filesystem::path obstaclesPath = constants::OBSTACLES_DIRECTORY;

    if (!std::filesystem::exists(obstaclesPath) ||
        !std::filesystem::is_directory(obstaclesPath)) {
        return obstacles;
    }

    for (const auto& entry : std::filesystem::directory_iterator(obstaclesPath)) {
        if (entry.is_regular_file() && entry.path().extension() ==
            constants::LEVEL_FILE_EXTENSION) {
            try {
                std::ifstream file(entry.path());
                nlohmann::json obstacleData;
                file >> obstacleData;
                file.close();

                if (obstacleData.contains(constants::NAME_FIELD) &&
                    obstacleData[constants::NAME_FIELD].is_string()) {
                    std::string obstacleName = obstacleData[constants::NAME_FIELD];
                    obstacles.push_back(obstacleName);
                }
            } catch (const std::exception&) {
            }
        }
    }

    return obstacles;
}

std::vector<std::string> LevelEditorState::loadAvailablePowerUps() {
    std::vector<std::string> powerUps;
    std::filesystem::path powerUpsPath = constants::POWERUPS_DIRECTORY;

    if (!std::filesystem::exists(powerUpsPath) ||
        !std::filesystem::is_directory(powerUpsPath)) {
        return powerUps;
    }

    for (const auto& entry : std::filesystem::directory_iterator(powerUpsPath)) {
        if (entry.is_regular_file() && entry.path().extension() ==
            constants::LEVEL_FILE_EXTENSION) {
            try {
                std::ifstream file(entry.path());
                nlohmann::json powerUpData;
                file >> powerUpData;
                file.close();

                if (powerUpData.contains(constants::NAME_FIELD) &&
                    powerUpData[constants::NAME_FIELD].is_string()) {
                    std::string powerUpName = powerUpData[constants::NAME_FIELD];
                    powerUps.push_back(powerUpName);
                }
            } catch (const std::exception&) {
            }
        }
    }

    return powerUps;
}

std::vector<std::string> LevelEditorState::loadAvailableEnemies() {
    std::vector<std::string> enemies;
    std::filesystem::path enemiesPath = constants::ENEMIES_DIRECTORY;

    if (!std::filesystem::exists(enemiesPath) ||
        !std::filesystem::is_directory(enemiesPath)) {
        return enemies;
    }

    for (const auto& entry : std::filesystem::directory_iterator(enemiesPath)) {
        if (entry.is_regular_file() && entry.path().extension() ==
            constants::LEVEL_FILE_EXTENSION) {
            try {
                std::ifstream file(entry.path());
                nlohmann::json enemyData;
                file >> enemyData;
                file.close();

                if (enemyData.contains(constants::NAME_FIELD) &&
                    enemyData[constants::NAME_FIELD].is_string()) {
                    std::string enemyName = enemyData[constants::NAME_FIELD];
                    enemies.push_back(enemyName);
                }
            } catch (const std::exception&) {
            }
        }
    }

    return enemies;
}

LevelPreviewSprite LevelEditorState::extractSpriteDataFromPrefab(
    const std::string& prefabPath
) {
    LevelPreviewSprite result;
    result.texturePath = "";
    result.width = 0.0f;
    result.height = 0.0f;
    result.scale = 1.0f;
    result.rotation = 0.0f;

    try {
        std::ifstream file(prefabPath);
        if (!file.is_open()) {
            return result;
        }

        nlohmann::json prefabData;
        file >> prefabData;
        file.close();

        if (!prefabData.contains(constants::COMPONENTS_FIELD)) {
            return result;
        }

        const auto& components = prefabData[constants::COMPONENTS_FIELD];

        if (components.contains(constants::ANIMATIONCOMPONENT)) {
            const auto& animComponent = components[constants::ANIMATIONCOMPONENT];
            if (animComponent.contains(constants::TEXTUREPATH_FIELD)) {
                result.texturePath =
                    animComponent[constants::TEXTUREPATH_FIELD].get<std::string>();
            }

            result.isAnimation = true;
            if (animComponent.contains(constants::STATES_FIELD)) {
                const auto& states = animComponent[constants::STATES_FIELD];
                if (!states.empty()) {
                    auto it = states.begin();
                    const auto& firstState = it.value();

                    if (result.texturePath.empty() &&
                        firstState.contains(constants::TEXTUREPATH_FIELD)) {
                        result.texturePath = firstState[
                            constants::TEXTUREPATH_FIELD].get<std::string>();
                    }

                    result.frameCount = firstState.value(
                        constants::FRAMECOUNT_FIELD, 1.0f);
                    result.frameWidth = firstState.value(
                        constants::FRAMEWIDTH_FIELD, 0.0f);
                    result.frameHeight = firstState.value(
                        constants::FRAMEHEIGHT_FIELD, 0.0f);
                    result.animationSpeed = firstState.value(
                        constants::SPEED_FIELD, 0.1f);
                    result.animationLoop = firstState.value(
                        constants::LOOP_FIELD, true);
                }
            }
        } else if (components.contains(constants::SPRITECOMPONENT)) {
            const auto& spriteComponent = components[constants::SPRITECOMPONENT];
            if (spriteComponent.contains(constants::FILEPATH_FIELD)) {
                result.texturePath =
                    spriteComponent[constants::FILEPATH_FIELD].get<std::string>();
            }
        }

        if (components.contains(constants::COLLIDERCOMPONENT)) {
            const auto& colliderComponent = components[constants::COLLIDERCOMPONENT];
            if (colliderComponent.contains(constants::SIZE_FIELD)) {
                const auto& size = colliderComponent[constants::SIZE_FIELD];
                if (size.contains(constants::X_FIELD)) {
                    result.width = size[constants::X_FIELD].get<float>();
                }
                if (size.contains(constants::Y_FIELD)) {
                    result.height = size[constants::Y_FIELD].get<float>();
                }
            }
        }

        if (components.contains(constants::TRANSFORMCOMPONENT)) {
            const auto& transformComponent =
                components[constants::TRANSFORMCOMPONENT];
            if (transformComponent.contains(constants::SCALE_FIELD)) {
                const auto& scale = transformComponent[constants::SCALE_FIELD];
                if (scale.contains(constants::X_FIELD)) {
                    result.scale = scale[constants::X_FIELD].get<float>();
                }
            }
            if (transformComponent.contains(constants::ROTATION_FIELD)) {
                result.rotation =
                    transformComponent[constants::ROTATION_FIELD].get<float>();
            }
        }

        result.width *= result.scale;
        result.height *= result.scale;
    } catch (const std::exception&) {
    }

    return result;
}

void LevelEditorState::initializeViewport() {
    const float bottomPanelHeight = 200.0f;
    const float canvasHeight = constants::MAX_HEIGHT - bottomPanelHeight;
    _viewportZoom = canvasHeight / constants::MAX_HEIGHT;
    _viewportOffset = math::Vector2f(0.0f, 0.0f);
}

LevelEditorState::WorldCoordinates LevelEditorState::extractWorldCoordinates(
    const math::Vector2f& mousePos, float sidePanelWidth) const {
    WorldCoordinates coords;
    coords.levelX = sidePanelWidth - (_viewportOffset.getX() * _viewportZoom);
    coords.levelY = -(_viewportOffset.getY() * _viewportZoom);
    coords.worldX = (mousePos.getX() - coords.levelX) / _viewportZoom;
    coords.worldY = (mousePos.getY() - coords.levelY) / _viewportZoom;
    return coords;
}

}  // namespace gsm
