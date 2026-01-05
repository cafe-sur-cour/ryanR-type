/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ReplaySystem
*/

#include "ReplaySystem.hpp"
#include <fstream>
#include <iostream>
#include "../../../common/ECS/view/View.hpp"

namespace ecs {

ReplaySystem::ReplaySystem() : _totalElapsedTime(0.0f) {
    // Clear the replay file at startup
    std::ofstream file("replay.json", std::ios::trunc);
    file.close();
}

void ReplaySystem::update(std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<Registry> registry, float deltaTime) {
    (void)resourceManager;

    _totalElapsedTime += deltaTime;

    nlohmann::json frameData;
    frameData["totalTime"] = _totalElapsedTime;
    frameData["renderables"] = nlohmann::json::array();

    auto gameZoneView = registry->view<GameZoneComponent, TransformComponent>();
    auto gameZoneEntity = *gameZoneView.begin();
    auto gameZoneComp = registry->getComponent<GameZoneComponent>(gameZoneEntity);
    auto gameZoneTransform = registry->getComponent<TransformComponent>(gameZoneEntity);

    math::FRect gameZone = gameZoneComp->getZone();
    frameData["gamezone"] = nlohmann::json{
        {"x", gameZoneTransform->getPosition().getX()},
        {"y", gameZoneTransform->getPosition().getY()},
        {"width", gameZone.getWidth()},
        {"height", gameZone.getHeight()}
    };

    math::FRect gameZoneBounds = gameZone;
    gameZoneBounds.setLeft(gameZoneBounds.getLeft() + gameZoneTransform->getPosition().getX());
    gameZoneBounds.setTop(gameZoneBounds.getTop() + gameZoneTransform->getPosition().getY());

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
        renderable["transform"] = nlohmann::json{
            {"x", transform->getPosition().getX()},
            {"y", transform->getPosition().getY()},
            {"r", transform->getRotation()},
            {"sx", transform->getScale().getX()},
            {"sy", transform->getScale().getY()}
        };
        renderable["sprite"] = nlohmann::json{
            {"texture", getSpriteId(currentClip->texturePath)},
            {"offsetX", frameRect.getLeft()},
            {"offsetY", frameRect.getTop()},
            {"width", frameRect.getWidth()},
            {"height", frameRect.getHeight()}
        };

        frameData["renderables"].push_back(renderable);
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
        renderable["transform"] = nlohmann::json{
            {"x", transform->getPosition().getX()},
            {"y", transform->getPosition().getY()},
            {"r", transform->getRotation()},
            {"sx", transform->getScale().getX()},
            {"sy", transform->getScale().getY()}
        };
        renderable["sprite"] = nlohmann::json{
            {"texture", getSpriteId(sprite->getTexturePath())},
            {"offsetX", 0.0f},
            {"offsetY", 0.0f},
            {"width", 0.0f},
            {"height", 0.0f}
        };

        frameData["renderables"].push_back(renderable);
    }

    auto parallaxView = registry->view<ParallaxComponent, TransformComponent>();
    for (auto entity : parallaxView) {
        auto parallax = registry->getComponent<ParallaxComponent>(entity);
        auto transform = registry->getComponent<TransformComponent>(entity);

        if (!parallax || !transform) continue;

        nlohmann::json parallaxData;
        parallaxData["type"] = "parallax";
        parallaxData["transform"] = nlohmann::json{
            {"x", transform->getPosition().getX()},
            {"y", transform->getPosition().getY()},
            {"r", transform->getRotation()},
            {"sx", transform->getScale().getX()},
            {"sy", transform->getScale().getY()}
        };
        parallaxData["parallax"] = nlohmann::json{
            {"baseScrollSpeed", parallax->getBaseScrollSpeed()},
            {"direction", nlohmann::json{
                {"x", parallax->getDirection().getX()},
                {"y", parallax->getDirection().getY()}
            }}
        };

        nlohmann::json layersArray = nlohmann::json::array();
        for (const auto& layer : parallax->getLayers()) {
            nlohmann::json layerData;
            layerData["name"] = layer.name;
            layerData["filePath"] = layer.filePath;
            layerData["speedMultiplier"] = layer.speedMultiplier;
            layerData["scale"] = nlohmann::json{
                {"x", layer.scale.getX()},
                {"y", layer.scale.getY()}
            };
            layerData["scaleMode"] = static_cast<int>(layer.scaleMode);
            layerData["sourceSize"] = nlohmann::json{
                {"x", layer.sourceSize.getX()},
                {"y", layer.sourceSize.getY()}
            };
            layerData["repeat"] = layer.repeat;
            layerData["zIndex"] = layer.zIndex;
            layerData["currentOffset"] = nlohmann::json{
                {"x", layer.currentOffset.getX()},
                {"y", layer.currentOffset.getY()}
            };
            layersArray.push_back(layerData);
        }
        parallaxData["parallax"]["layers"] = layersArray;

        frameData["renderables"].push_back(parallaxData);
    }

    auto healthBarView = registry->view<HealthBarComponent, HealthComponent, TransformComponent, ColliderComponent>();
    for (auto entity : healthBarView) {
        auto healthBar = registry->getComponent<HealthBarComponent>(entity);
        auto health = registry->getComponent<HealthComponent>(entity);
        auto transform = registry->getComponent<TransformComponent>(entity);
        auto collider = registry->getComponent<ColliderComponent>(entity);

        if (!healthBar || !health || !transform || !collider) continue;

        nlohmann::json healthBarData;
        healthBarData["type"] = "healthbar";
        healthBarData["transform"] = nlohmann::json{
            {"x", transform->getPosition().getX()},
            {"y", transform->getPosition().getY()},
            {"r", transform->getRotation()},
            {"sx", transform->getScale().getX()},
            {"sy", transform->getScale().getY()}
        };
        healthBarData["health"] = nlohmann::json{
            {"current", health->getHealth()},
            {"max", health->getBaseHealth()}
        };
        healthBarData["collider"] = nlohmann::json{
            {"offsetX", collider->getOffset().getX()},
            {"offsetY", collider->getOffset().getY()},
            {"sizeX", collider->getSize().getX()},
            {"sizeY", collider->getSize().getY()}
        };

        frameData["renderables"].push_back(healthBarData);
    }

    auto textView = registry->view<TextComponent, TransformComponent>();
    for (auto entity : textView) {
        auto text = registry->getComponent<TextComponent>(entity);
        auto transform = registry->getComponent<TransformComponent>(entity);

        if (!text || !transform) continue;

        nlohmann::json textData;
        textData["type"] = "text";
        textData["transform"] = nlohmann::json{
            {"x", transform->getPosition().getX()},
            {"y", transform->getPosition().getY()},
            {"r", transform->getRotation()},
            {"sx", transform->getScale().getX()},
            {"sy", transform->getScale().getY()}
        };
        textData["text"] = nlohmann::json{
            {"content", text->getText()},
            {"color", nlohmann::json{
                {"r", text->getColor().r},
                {"g", text->getColor().g},
                {"b", text->getColor().b},
                {"a", text->getColor().a}
            }},
            {"fontPath", text->getFontPath()}
        };

        frameData["renderables"].push_back(textData);
    }

    auto rectangleView = registry->view<RectangleRenderComponent, TransformComponent>();
    for (auto entity : rectangleView) {
        auto rectangle = registry->getComponent<RectangleRenderComponent>(entity);
        auto transform = registry->getComponent<TransformComponent>(entity);

        if (!rectangle || !transform) continue;

        nlohmann::json rectangleData;
        rectangleData["type"] = "rectangle";
        rectangleData["transform"]["x"] = transform->getPosition().getX();
        rectangleData["transform"]["y"] = transform->getPosition().getY();
        rectangleData["transform"]["r"] = transform->getRotation();
        rectangleData["transform"]["sx"] = transform->getScale().getX();
        rectangleData["transform"]["sy"] = transform->getScale().getY();
        rectangleData["rectangle"]["width"] = rectangle->getWidth();
        rectangleData["rectangle"]["height"] = rectangle->getHeight();
        rectangleData["rectangle"]["color"]["r"] = rectangle->getColor().r;
        rectangleData["rectangle"]["color"]["g"] = rectangle->getColor().g;
        rectangleData["rectangle"]["color"]["b"] = rectangle->getColor().b;
        rectangleData["rectangle"]["color"]["a"] = rectangle->getColor().a;

        frameData["renderables"].push_back(rectangleData);
    }

    auto hitboxView = registry->view<HitboxRenderComponent, TransformComponent, ColliderComponent>();
    for (auto entity : hitboxView) {
        auto hitbox = registry->getComponent<HitboxRenderComponent>(entity);
        auto transform = registry->getComponent<TransformComponent>(entity);
        auto collider = registry->getComponent<ColliderComponent>(entity);

        if (!hitbox || !transform || !collider) continue;

        nlohmann::json hitboxData;
        hitboxData["type"] = "hitbox";
        hitboxData["transform"]["x"] = transform->getPosition().getX();
        hitboxData["transform"]["y"] = transform->getPosition().getY();
        hitboxData["transform"]["r"] = transform->getRotation();
        hitboxData["transform"]["sx"] = transform->getScale().getX();
        hitboxData["transform"]["sy"] = transform->getScale().getY();
        hitboxData["hitbox"]["color"]["r"] = hitbox->getColor().r;
        hitboxData["hitbox"]["color"]["g"] = hitbox->getColor().g;
        hitboxData["hitbox"]["color"]["b"] = hitbox->getColor().b;
        hitboxData["hitbox"]["color"]["a"] = hitbox->getColor().a;
        hitboxData["collider"]["offsetX"] = collider->getOffset().getX();
        hitboxData["collider"]["offsetY"] = collider->getOffset().getY();
        hitboxData["collider"]["sizeX"] = collider->getSize().getX();
        hitboxData["collider"]["sizeY"] = collider->getSize().getY();

        frameData["renderables"].push_back(hitboxData);
    }

    auto soundView = registry->view<SoundIntentComponent>();
    for (auto entity : soundView) {
        auto soundIntent = registry->getComponent<SoundIntentComponent>(entity);
        if (soundIntent) {
            nlohmann::json soundData;
            soundData["type"] = "sound";
            soundData["soundPath"] = soundIntent->getSoundPath();
            soundData["volume"] = soundIntent->getVolume();
            frameData["audio"].push_back(soundData);
        }
    }

    saveReplayToFile(frameData);
}

void ReplaySystem::saveReplayToFile(const nlohmann::json& frameData) {
    std::ofstream file("replay.json", std::ios::app);
    if (file.is_open()) {
        file << frameData.dump() << std::endl;
        file.close();
    } else {
        std::cerr << "Failed to save frame" << std::endl;
    }
}

std::string ReplaySystem::getSpriteId(const std::string& texturePath) {
    return texturePath;
}

}  // namespace ecs
