/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ReplaySystem
*/

#include "ReplaySystem.hpp"
#include <fstream>
#include <iostream>
#include <filesystem>  // NOLINT(build/c++17)
#include <memory>
#include <unordered_set>
#include <string>
#include <vector>
#include <algorithm>
#include "../../../common/ECS/view/View.hpp"
#include "../../constants.hpp"

namespace ecs {

ReplaySystem::ReplaySystem() : _totalElapsedTime(0.0f) {
    std::filesystem::path replayDir = constants::REPLAY_DIRECTORY;

    try {
        std::filesystem::create_directories(replayDir);
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Failed to create replay directories: " << e.what() << std::endl;
        return;
    }

    _currentReplayFile = getNextReplayFile();
    std::ofstream file(_currentReplayFile, std::ios::trunc);
    file.close();
}

void ReplaySystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime
) {
    (void)resourceManager;

    _totalElapsedTime += deltaTime;

    nlohmann::json frameData;
    frameData[constants::REPLAY_TOTAL_TIME] = _totalElapsedTime;
    frameData[constants::REPLAY_RENDERABLES] = nlohmann::json::array();

    auto gameZoneView = registry->view<GameZoneComponent, TransformComponent>();
    if (gameZoneView.begin() == gameZoneView.end()) {
        return;
    }
    auto gameZoneEntity = *gameZoneView.begin();
    auto gameZoneComp = registry->getComponent<GameZoneComponent>(gameZoneEntity);
    auto gameZoneTransform = registry->getComponent<TransformComponent>(gameZoneEntity);

    if (!gameZoneComp || !gameZoneTransform) {
        return;
    }

    math::FRect gameZone = gameZoneComp->getZone();
    frameData[constants::REPLAY_GAMEZONE] = nlohmann::json{
        {constants::REPLAY_X, gameZoneTransform->getPosition().getX()},
        {constants::REPLAY_Y, gameZoneTransform->getPosition().getY()},
        {constants::REPLAY_WIDTH, gameZone.getWidth()},
        {constants::REPLAY_HEIGHT, gameZone.getHeight()}
    };

    std::unordered_set<ecs::Entity> processedEntities;

    auto animationView = registry->view<AnimationComponent, TransformComponent>();
    for (auto entity : animationView) {
        auto animation = registry->getComponent<AnimationComponent>(entity);
        auto transform = registry->getComponent<TransformComponent>(entity);

        if (!animation || !transform || !animation->isValid()) continue;

        auto currentClip = animation->getCurrentClip();
        if (!currentClip) continue;

        auto collider = registry->getComponent<ColliderComponent>(entity);
        math::FRect entityRect;
        if (collider) {
            entityRect = math::FRect(
                transform->getPosition().getX() + collider->getOffset().getX(),
                transform->getPosition().getY() + collider->getOffset().getY(),
                collider->getSize().getX(),
                collider->getSize().getY()
            );
        } else {
            entityRect = math::FRect(
                transform->getPosition().getX() - 10.0f,
                transform->getPosition().getY() - 10.0f,
                20.0f,
                20.0f
            );
        }

        const math::FRect& frameRect = animation->getFrameRect();

        nlohmann::json renderable;
        renderable[constants::REPLAY_TRANSFORM] = nlohmann::json{
            {constants::REPLAY_X, transform->getPosition().getX()},
            {constants::REPLAY_Y, transform->getPosition().getY()},
            {constants::REPLAY_ROTATION, transform->getRotation()},
            {constants::REPLAY_SCALE_X, transform->getScale().getX()},
            {constants::REPLAY_SCALE_Y, transform->getScale().getY()}
        };
        renderable[constants::REPLAY_SPRITE] = nlohmann::json{
            {constants::REPLAY_TEXTURE, getSpriteId(currentClip->texturePath)},
            {constants::REPLAY_OFFSET_X, frameRect.getLeft()},
            {constants::REPLAY_OFFSET_Y, frameRect.getTop()},
            {constants::REPLAY_WIDTH, frameRect.getWidth()},
            {constants::REPLAY_HEIGHT, frameRect.getHeight()}
        };

        frameData[constants::REPLAY_RENDERABLES].push_back(renderable);
        processedEntities.insert(entity);
    }

    auto spriteView = registry->view<SpriteComponent, TransformComponent>();
    for (auto entity : spriteView) {
        if (processedEntities.find(entity) != processedEntities.end()) continue;
        auto sprite = registry->getComponent<SpriteComponent>(entity);
        auto transform = registry->getComponent<TransformComponent>(entity);

        if (!sprite || !transform || !sprite->isValid()) continue;

        auto collider = registry->getComponent<ColliderComponent>(entity);
        math::FRect entityRect;
        if (collider) {
            entityRect = math::FRect(
                transform->getPosition().getX() + collider->getOffset().getX(),
                transform->getPosition().getY() + collider->getOffset().getY(),
                collider->getSize().getX(),
                collider->getSize().getY()
            );
        } else {
            entityRect = math::FRect(
                transform->getPosition().getX() - 10.0f,
                transform->getPosition().getY() - 10.0f,
                20.0f,
                20.0f
            );
        }

        nlohmann::json renderable;
        renderable[constants::REPLAY_TRANSFORM] = nlohmann::json{
            {constants::REPLAY_X, transform->getPosition().getX()},
            {constants::REPLAY_Y, transform->getPosition().getY()},
            {constants::REPLAY_ROTATION, transform->getRotation()},
            {constants::REPLAY_SCALE_X, transform->getScale().getX()},
            {constants::REPLAY_SCALE_Y, transform->getScale().getY()}
        };
        renderable[constants::REPLAY_SPRITE] = nlohmann::json{
            {constants::REPLAY_TEXTURE, getSpriteId(sprite->getTexturePath())},
            {constants::REPLAY_OFFSET_X, 0.0f},
            {constants::REPLAY_OFFSET_Y, 0.0f},
            {constants::REPLAY_WIDTH, 0.0f},
            {constants::REPLAY_HEIGHT, 0.0f}
        };

        frameData[constants::REPLAY_RENDERABLES].push_back(renderable);
    }

    auto parallaxView = registry->view<ParallaxComponent, TransformComponent>();
    for (auto entity : parallaxView) {
        auto parallax = registry->getComponent<ParallaxComponent>(entity);
        auto transform = registry->getComponent<TransformComponent>(entity);

        if (!parallax || !transform) continue;

        nlohmann::json parallaxData;
        parallaxData[constants::REPLAY_TYPE] = constants::REPLAY_TYPE_PARALLAX;
        parallaxData[constants::REPLAY_TRANSFORM] = nlohmann::json{
            {constants::REPLAY_X, transform->getPosition().getX()},
            {constants::REPLAY_Y, transform->getPosition().getY()},
            {constants::REPLAY_ROTATION, transform->getRotation()},
            {constants::REPLAY_SCALE_X, transform->getScale().getX()},
            {constants::REPLAY_SCALE_Y, transform->getScale().getY()}
        };
        parallaxData[constants::REPLAY_PARALLAX] = nlohmann::json{
            {constants::REPLAY_BASE_SCROLL_SPEED, parallax->getBaseScrollSpeed()},
            {constants::REPLAY_DIRECTION, nlohmann::json{
                {constants::REPLAY_X, parallax->getDirection().getX()},
                {constants::REPLAY_Y, parallax->getDirection().getY()}
            }}
        };

        nlohmann::json layersArray = nlohmann::json::array();
        for (const auto& layer : parallax->getLayers()) {
            nlohmann::json layerData;
            layerData[constants::REPLAY_NAME] = layer->name;
            layerData[constants::REPLAY_FILE_PATH] = layer->filePath;
            layerData[constants::REPLAY_SPEED_MULTIPLIER] = layer->speedMultiplier;
            layerData[constants::REPLAY_SCALE] = nlohmann::json{
                {constants::REPLAY_X, layer->scale.getX()},
                {constants::REPLAY_Y, layer->scale.getY()}
            };
            layerData[constants::REPLAY_SCALE_MODE] = static_cast<int>(layer->scaleMode);
            layerData[constants::REPLAY_SOURCE_SIZE] = nlohmann::json{
                {constants::REPLAY_X, layer->sourceSize.getX()},
                {constants::REPLAY_Y, layer->sourceSize.getY()}
            };
            layerData[constants::REPLAY_REPEAT] = layer->repeat;
            layerData[constants::REPLAY_Z_INDEX] = layer->zIndex;
            layerData[constants::REPLAY_CURRENT_OFFSET] = nlohmann::json{
                {constants::REPLAY_X, layer->currentOffset.getX()},
                {constants::REPLAY_Y, layer->currentOffset.getY()}
            };
            layersArray.push_back(layerData);
        }
        parallaxData[constants::REPLAY_PARALLAX][constants::REPLAY_LAYERS] = layersArray;

        frameData[constants::REPLAY_RENDERABLES].push_back(parallaxData);
    }

    auto healthBarView = registry->view<
        HealthBarComponent, HealthComponent, TransformComponent, ColliderComponent>();
    for (auto entity : healthBarView) {
        auto healthBar = registry->getComponent<HealthBarComponent>(entity);
        auto health = registry->getComponent<HealthComponent>(entity);
        auto transform = registry->getComponent<TransformComponent>(entity);
        auto collider = registry->getComponent<ColliderComponent>(entity);

        if (!healthBar || !health || !transform || !collider) continue;

        nlohmann::json healthBarData;
        healthBarData[constants::REPLAY_TYPE] = constants::REPLAY_TYPE_HEALTHBAR;
        healthBarData[constants::REPLAY_TRANSFORM] = nlohmann::json{
            {constants::REPLAY_X, transform->getPosition().getX()},
            {constants::REPLAY_Y, transform->getPosition().getY()},
            {constants::REPLAY_ROTATION, transform->getRotation()},
            {constants::REPLAY_SCALE_X, transform->getScale().getX()},
            {constants::REPLAY_SCALE_Y, transform->getScale().getY()}
        };
        healthBarData[constants::REPLAY_HEALTH] = nlohmann::json{
            {constants::REPLAY_CURRENT, health->getHealth()},
            {constants::REPLAY_MAX, health->getBaseHealth()}
        };
        healthBarData[constants::REPLAY_COLLIDER] = nlohmann::json{
            {constants::REPLAY_OFFSET_X, collider->getOffset().getX()},
            {constants::REPLAY_OFFSET_Y, collider->getOffset().getY()},
            {constants::REPLAY_SIZE_X, collider->getSize().getX()},
            {constants::REPLAY_SIZE_Y, collider->getSize().getY()}
        };

        frameData[constants::REPLAY_RENDERABLES].push_back(healthBarData);
    }

    auto textView = registry->view<TextComponent, TransformComponent>();
    for (auto entity : textView) {
        auto text = registry->getComponent<TextComponent>(entity);
        auto transform = registry->getComponent<TransformComponent>(entity);

        if (!text || !transform) continue;

        nlohmann::json textData;
        textData[constants::REPLAY_TYPE] = constants::REPLAY_TYPE_TEXT;
        textData[constants::REPLAY_TRANSFORM] = nlohmann::json{
            {constants::REPLAY_X, transform->getPosition().getX()},
            {constants::REPLAY_Y, transform->getPosition().getY()},
            {constants::REPLAY_ROTATION, transform->getRotation()},
            {constants::REPLAY_SCALE_X, transform->getScale().getX()},
            {constants::REPLAY_SCALE_Y, transform->getScale().getY()}
        };
        textData[constants::REPLAY_TEXT] = nlohmann::json{
            {constants::REPLAY_CONTENT, text->getText()},
            {constants::REPLAY_COLOR, nlohmann::json{
                {constants::REPLAY_RED, text->getColor().r},
                {constants::REPLAY_GREEN, text->getColor().g},
                {constants::REPLAY_BLUE, text->getColor().b},
                {constants::REPLAY_ALPHA, text->getColor().a}
            }},
            {constants::REPLAY_FONT_PATH, text->getFontPath()}
        };

        frameData[constants::REPLAY_RENDERABLES].push_back(textData);
    }

    auto rectangleView = registry->view<RectangleRenderComponent, TransformComponent>();
    for (auto entity : rectangleView) {
        auto rectangle = registry->getComponent<RectangleRenderComponent>(entity);
        auto transform = registry->getComponent<TransformComponent>(entity);

        if (!rectangle || !transform) continue;

        nlohmann::json rectangleData;
        rectangleData[constants::REPLAY_TYPE] = constants::REPLAY_TYPE_RECTANGLE;
        rectangleData[constants::REPLAY_TRANSFORM][constants::REPLAY_X] =
            transform->getPosition().getX();
        rectangleData[constants::REPLAY_TRANSFORM][constants::REPLAY_Y] =
            transform->getPosition().getY();
        rectangleData[constants::REPLAY_TRANSFORM][constants::REPLAY_ROTATION] =
            transform->getRotation();
        rectangleData[constants::REPLAY_TRANSFORM][constants::REPLAY_SCALE_X] =
            transform->getScale().getX();
        rectangleData[constants::REPLAY_TRANSFORM][constants::REPLAY_SCALE_Y] =
            transform->getScale().getY();
        rectangleData[constants::REPLAY_RECTANGLE][constants::REPLAY_WIDTH] =
            rectangle->getWidth();
        rectangleData[constants::REPLAY_RECTANGLE][constants::REPLAY_HEIGHT] =
            rectangle->getHeight();
        rectangleData[constants::REPLAY_RECTANGLE][constants::REPLAY_COLOR]
            [constants::REPLAY_RED] = rectangle->getColor().r;
        rectangleData[constants::REPLAY_RECTANGLE][constants::REPLAY_COLOR]
            [constants::REPLAY_GREEN] = rectangle->getColor().g;
        rectangleData[constants::REPLAY_RECTANGLE][constants::REPLAY_COLOR]
            [constants::REPLAY_BLUE] = rectangle->getColor().b;
        rectangleData[constants::REPLAY_RECTANGLE][constants::REPLAY_COLOR]
            [constants::REPLAY_ALPHA] = rectangle->getColor().a;

        frameData[constants::REPLAY_RENDERABLES].push_back(rectangleData);
    }

    auto hitboxView = registry->view<
        HitboxRenderComponent, TransformComponent, ColliderComponent>();
    for (auto entity : hitboxView) {
        auto hitbox = registry->getComponent<HitboxRenderComponent>(entity);
        auto transform = registry->getComponent<TransformComponent>(entity);
        auto collider = registry->getComponent<ColliderComponent>(entity);

        if (!hitbox || !transform || !collider) continue;

        nlohmann::json hitboxData;
        hitboxData[constants::REPLAY_TYPE] = constants::REPLAY_TYPE_HITBOX;
        hitboxData[constants::REPLAY_TRANSFORM][constants::REPLAY_X] =
            transform->getPosition().getX();
        hitboxData[constants::REPLAY_TRANSFORM][constants::REPLAY_Y] =
            transform->getPosition().getY();
        hitboxData[constants::REPLAY_TRANSFORM][constants::REPLAY_ROTATION] =
            transform->getRotation();
        hitboxData[constants::REPLAY_TRANSFORM][constants::REPLAY_SCALE_X] =
            transform->getScale().getX();
        hitboxData[constants::REPLAY_TRANSFORM][constants::REPLAY_SCALE_Y] =
            transform->getScale().getY();
        hitboxData[constants::REPLAY_HITBOX][constants::REPLAY_COLOR]
            [constants::REPLAY_RED] = hitbox->getColor().r;
        hitboxData[constants::REPLAY_HITBOX][constants::REPLAY_COLOR]
            [constants::REPLAY_GREEN] = hitbox->getColor().g;
        hitboxData[constants::REPLAY_HITBOX][constants::REPLAY_COLOR]
            [constants::REPLAY_BLUE] = hitbox->getColor().b;
        hitboxData[constants::REPLAY_HITBOX][constants::REPLAY_COLOR]
            [constants::REPLAY_ALPHA] = hitbox->getColor().a;
        hitboxData[constants::REPLAY_COLLIDER][constants::REPLAY_OFFSET_X] =
            collider->getOffset().getX();
        hitboxData[constants::REPLAY_COLLIDER][constants::REPLAY_OFFSET_Y] =
            collider->getOffset().getY();
        hitboxData[constants::REPLAY_COLLIDER][constants::REPLAY_SIZE_X] =
            collider->getSize().getX();
        hitboxData[constants::REPLAY_COLLIDER][constants::REPLAY_SIZE_Y] =
            collider->getSize().getY();

        frameData[constants::REPLAY_RENDERABLES].push_back(hitboxData);
    }

    auto soundView = registry->view<SoundIntentComponent>();
    for (auto entity : soundView) {
        auto soundIntent = registry->getComponent<SoundIntentComponent>(entity);
        if (soundIntent) {
            nlohmann::json soundData;
            soundData[constants::REPLAY_TYPE] = constants::REPLAY_TYPE_SOUND;
            soundData[constants::REPLAY_SOUND_PATH] = soundIntent->getSoundPath();
            soundData[constants::REPLAY_VOLUME] = soundIntent->getVolume();
            frameData[constants::REPLAY_AUDIO].push_back(soundData);
        }
    }

    saveReplayToFile(frameData);
}

void ReplaySystem::saveReplayToFile(const nlohmann::json& frameData) {
    std::ofstream file(_currentReplayFile, std::ios::app);
    if (file.is_open()) {
        file << frameData.dump() << std::endl;
        file.close();
    } else {
        std::cerr << "Failed to save frame to " << _currentReplayFile << std::endl;
    }
}

std::filesystem::path ReplaySystem::getNextReplayFile() {
    std::filesystem::path replayDir = constants::REPLAY_DIRECTORY;

    std::vector<std::filesystem::path> existingReplays;
    if (std::filesystem::exists(replayDir)) {
        for (const auto& entry : std::filesystem::directory_iterator(replayDir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".json" &&
                entry.path().stem().string().find(constants::REPLAY_FILE_PREFIX) == 0) {
                existingReplays.push_back(entry.path());
            }
        }
    }

    if (existingReplays.size() < constants::MAX_REPLAY_FILES) {
        return replayDir / (constants::REPLAY_FILE_PREFIX +
            std::to_string(existingReplays.size() + 1) + ".json");
    } else {
        std::sort(existingReplays.begin(), existingReplays.end(),
            [](const std::filesystem::path& a, const std::filesystem::path& b) {
                return std::filesystem::last_write_time(a) <
                    std::filesystem::last_write_time(b);
            });
        return existingReplays[0];
    }
}

std::string ReplaySystem::getSpriteId(const std::string& texturePath) {
    return texturePath;
}

}  // namespace ecs
