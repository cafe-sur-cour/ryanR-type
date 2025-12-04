/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ComponentCreators
*/

#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <vector>
#include <utility>
#include <functional>
#include "Parser.hpp"
#include "../constants.hpp"
#include "../components/tags/ShooterTag.hpp"
#include "../components/tags/ProjectileTag.hpp"
#include "../components/tags/ProjectilePassThroughTag.hpp"
#include "../components/permanent/ShootingStatsComponent.hpp"
#include "../components/permanent/ProjectilePrefabComponent.hpp"
#include "../components/permanent/VelocityComponent.hpp"
#include "../../client/components/rendering/RectangleRenderComponent.hpp"
#include "../../client/components/rendering/TextComponent.hpp"
#include "../components/permanent/LifetimeComponent.hpp"
#include "../../client/components/rendering/AnimationComponent.hpp"
#include "../../client/components/rendering/MusicComponent.hpp"
#include "../../client/components/rendering/ParallaxComponent.hpp"
#include "../ECS/entity/Entity.hpp"
#include "../ECS/entity/registry/Registry.hpp"
#include "../../client/components/tags/BackGroundMusicTag.hpp"
#include "../components/tags/ScoreTag.hpp"
#include "../components/permanent/ScoreComponent.hpp"

void Parser::instanciateComponentDefinitions() {
    std::map<std::string, std::pair<std::type_index,
    std::vector<Field>>> componentDefinitions = {
        {constants::TRANSFORMCOMPONENT, {std::type_index(typeid(ecs::TransformComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING},
            {constants::POSITION_FIELD, FieldType::VECTOR2F},
            {constants::SCALE_FIELD, FieldType::VECTOR2F},
            {constants::ROTATION_FIELD, FieldType::FLOAT}
        }}},
        {constants::VELOCITYCOMPONENT, {std::type_index(typeid(ecs::VelocityComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING}
        }}},
        {constants::SPEEDCOMPONENT, {std::type_index(typeid(ecs::SpeedComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING},
            {constants::SPEED_FIELD, FieldType::FLOAT}
        }}},
        {constants::SPRITECOMPONENT, {std::type_index(typeid(ecs::SpriteComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING},
            {constants::FILEPATH_FIELD, FieldType::STRING}
        }}},
        {constants::ANIMATIONCOMPONENT, {std::type_index(typeid(ecs::AnimationComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING},
            {constants::STATES_FIELD, FieldType::JSON},
            {constants::INITIALSTATE_FIELD, FieldType::STRING},
            {constants::TRANSITIONS_FIELD, FieldType::JSON}
        }}},
        {constants::CONTROLLABLETAG, {std::type_index(typeid(ecs::ControllableTag)), {
            {constants::TARGET_FIELD, FieldType::STRING}
        }}},
        {constants::PLAYERTAG, {std::type_index(typeid(ecs::PlayerTag)), {
            {constants::TARGET_FIELD, FieldType::STRING}
        }}},
        {constants::SHOOTERTAG, {std::type_index(typeid(ecs::ShooterTag)), {
            {constants::TARGET_FIELD, FieldType::STRING}
        }}},
        {constants::PROJECTILETAG, {std::type_index(typeid(ecs::ProjectileTag)), {
            {constants::TARGET_FIELD, FieldType::STRING}
        }}},
        {constants::PROJECTILEPASSTHROUGHTAG,
            {std::type_index(typeid(ecs::ProjectilePassThroughTag)), {
            {constants::TARGET_FIELD, FieldType::STRING}
        }}},
        {constants::COLLIDERCOMPONENT, {std::type_index(typeid(ecs::ColliderComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING},
            {constants::OFFSET_FIELD, FieldType::VECTOR2F},
            {constants::SIZE_FIELD, FieldType::VECTOR2F},
            {constants::TYPE_FIELD, FieldType::STRING}
        }}},
        {constants::SHOOTINGSTATSCOMPONENT,
            {std::type_index(typeid(ecs::ShootingStatsComponent)), {
                {constants::TARGET_FIELD, FieldType::STRING},
                {constants::FIRERATE_FIELD, FieldType::FLOAT},
                {constants::PROJECTILESPEED_FIELD, FieldType::FLOAT},
                {constants::SHOTCOUNT_FIELD, FieldType::INT},
                {constants::ANGLEOFFSET_FIELD, FieldType::FLOAT},
                {constants::SPREADANGLE_FIELD, FieldType::FLOAT}
            }
        }},
        {constants::RECTANGLERENDERCOMPONENT, {
            std::type_index(typeid(ecs::RectangleRenderComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING},
            {constants::WIDTH_FIELD, FieldType::FLOAT},
            {constants::HEIGHT_FIELD, FieldType::FLOAT},
            {constants::COLOR_FIELD, FieldType::OBJECT}
        }}},
        {constants::TEXTCOMPONENT, {std::type_index(typeid(ecs::TextComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING},
            {constants::TEXT_FIELD, FieldType::STRING},
            {constants::FONTPATH_FIELD, FieldType::STRING},
            {constants::COLOR_FIELD, FieldType::OBJECT}
        }}},
        {constants::PROJECTILEPREFABCOMPONENT, {
            std::type_index(typeid(ecs::ProjectilePrefabComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING},
            {constants::PREFABNAME_FIELD, FieldType::STRING}
        }}},
        {constants::LIFETIMECOMPONENT, {
            std::type_index(typeid(ecs::LifetimeComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING},
            {constants::LIFETIME_FIELD, FieldType::FLOAT}
        }}},
        {constants::PARALLAXCOMPONENT, {
            std::type_index(typeid(ecs::ParallaxComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING},
            {constants::BASESCROLLSPEED_FIELD, FieldType::FLOAT},
            {constants::DIRECTION_FIELD, FieldType::VECTOR2F},
            {constants::LAYERS_FIELD, FieldType::JSON}
        }}},
                {constants::BACKGROUNDMUSICTAG, {
            std::type_index(typeid(ecs::BackGroundMusicTag)), {
            {constants::TARGET_FIELD, FieldType::STRING}
        }}},
        {constants::MUSICCOMPONENT, {
            std::type_index(typeid(ecs::MusicComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING},
            {constants::MUSICFILE_FIELD, FieldType::STRING},
            {constants::INITIALSTATEMUSIC_FIELD, FieldType::STRING},
            {constants::VOLUME_FIELD, FieldType::FLOAT},
            {constants::LOOP_FIELD, FieldType::BOOL}
        }}},
        {constants::SCORETAG, {
            std::type_index(typeid(ecs::ScoreTag)), {
            {constants::TARGET_FIELD, FieldType::STRING}
        }}},
        {constants::SCORECOMPONENT, {
            std::type_index(typeid(ecs::ScoreComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING},
            {constants::SCORE_FIELD, FieldType::INT}
        }}}
    };
    _componentDefinitions = std::make_shared<std::map<std::string,
        std::pair<std::type_index, std::vector<Field>>>>(componentDefinitions);
}

void Parser::instanciateComponentCreators() {
    registerComponent<ecs::TransformComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        auto pos = std::get<math::Vector2f>(*fields.at(constants::POSITION_FIELD));
        auto scale = std::get<math::Vector2f>(*fields.at(constants::SCALE_FIELD));
        auto rot = std::get<float>(*fields.at(constants::ROTATION_FIELD));
        return std::make_shared<ecs::TransformComponent>(pos, rot, scale);
    });

    registerComponent<ecs::VelocityComponent>([]([[maybe_unused]] const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        return std::make_shared<ecs::VelocityComponent>();
    });

    registerComponent<ecs::SpeedComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        auto speed = std::get<float>(*fields.at(constants::SPEED_FIELD));
        return std::make_shared<ecs::SpeedComponent>(speed);
    });

    registerComponent<ecs::SpriteComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        auto path = std::get<std::string>(*fields.at(constants::FILEPATH_FIELD));
        return std::make_shared<ecs::SpriteComponent>(path);
    });

    registerComponent<ecs::AnimationComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        auto statesJson = std::get<nlohmann::json>(*fields.at(constants::STATES_FIELD));
        auto initialState = std::get<std::string>(*fields.at(constants::INITIALSTATE_FIELD));

        auto anim = std::make_shared<ecs::AnimationComponent>();

        for (auto& [stateName, stateData] : statesJson.items()) {
            std::string texturePath = stateData[constants::TEXTUREPATH_FIELD];
            float frameWidth = stateData[constants::FRAMEWIDTH_FIELD];
            float frameHeight = stateData[constants::FRAMEHEIGHT_FIELD];
            int frameCount = stateData[constants::FRAMECOUNT_FIELD];
            float startWidth = stateData[constants::STARTWIDTH_FIELD];
            float startHeight = stateData[constants::STARTHEIGHT_FIELD];
            float speed = stateData.value(constants::SPEED_FIELD, 0.1f);
            bool loop = stateData.value(constants::LOOP_FIELD, true);

            ecs::AnimationClip clip{
                texturePath, frameWidth, frameHeight,
                frameCount, startWidth, startHeight, speed, loop};
            anim->addState(stateName, std::make_shared<ecs::AnimationClip>(clip));
        }

        anim->setCurrentState(initialState);

        if (fields.count(constants::TRANSITIONS_FIELD)) {
            auto transitionsJson =
                std::get<nlohmann::json>(*fields.at(constants::TRANSITIONS_FIELD));
            for (auto& transitionData : transitionsJson) {
                std::string from = transitionData[constants::FROM_FIELD];
                std::string to = transitionData[constants::TO_FIELD];
                bool rewind = transitionData[constants::REWIND_FIELD];

                std::vector<ecs::AnimationCondition> conditions;
                auto conditionsJson = transitionData[constants::CONDITIONS_FIELD];
                for (auto& condData : conditionsJson) {
                    std::string param = condData[constants::PARAM_FIELD];
                    bool equals = condData[constants::EQUALS_FIELD];
                    conditions.push_back({param, equals});
                }

                anim->addTransition(from, to, conditions, rewind);
            }
        }

        return anim;
    });

    registerComponent<ecs::ControllableTag>([]([[maybe_unused]] const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        return std::make_shared<ecs::ControllableTag>();
    });

    registerComponent<ecs::PlayerTag>([]([[maybe_unused]] const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        return std::make_shared<ecs::PlayerTag>();
    });

    registerComponent<ecs::ShooterTag>([]([[maybe_unused]] const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        return std::make_shared<ecs::ShooterTag>();
    });

    registerComponent<ecs::ProjectileTag>([]([[maybe_unused]] const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        return std::make_shared<ecs::ProjectileTag>();
    });

    registerComponent<
    ecs::ProjectilePassThroughTag>([]([[maybe_unused]] const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        return std::make_shared<ecs::ProjectilePassThroughTag>();
    });

    registerComponent<ecs::ColliderComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        auto offset = std::get<math::Vector2f>(*fields.at(constants::OFFSET_FIELD));
        auto size = std::get<math::Vector2f>(*fields.at(constants::SIZE_FIELD));
        auto typeStr = std::get<std::string>(*fields.at(constants::TYPE_FIELD));

        ecs::CollisionType type = ecs::CollisionType::Solid;
        if (typeStr == constants::COLLISION_TYPE_SOLID) {
            type = ecs::CollisionType::Solid;
        } else if (typeStr == constants::COLLISION_TYPE_TRIGGER) {
            type = ecs::CollisionType::Trigger;
        } else if (typeStr == constants::COLLISION_TYPE_PUSH) {
            type = ecs::CollisionType::Push;
        } else if (typeStr == constants::COLLISION_TYPE_NONE) {
            type = ecs::CollisionType::None;
        }

        return std::make_shared<ecs::ColliderComponent>(offset, size, type);
    });

    registerComponent<ecs::ShootingStatsComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        auto fireRate = std::get<float>(*fields.at(constants::FIRERATE_FIELD));
        auto projectileSpeed = std::get<float>(*fields.at(constants::PROJECTILESPEED_FIELD));
        auto shotCount = std::get<int>(*fields.at(constants::SHOTCOUNT_FIELD));
        auto angleOffset = std::get<float>(*fields.at(constants::ANGLEOFFSET_FIELD));
        auto spreadAngle = std::get<float>(*fields.at(constants::SPREADANGLE_FIELD));
        ecs::MultiShotPattern pattern(shotCount, spreadAngle, angleOffset);
        return std::make_shared<ecs::ShootingStatsComponent>(
            fireRate, projectileSpeed, pattern
        );
    });

    registerComponent<ecs::RectangleRenderComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        auto width = std::get<float>(*fields.at(constants::WIDTH_FIELD));
        auto height = std::get<float>(*fields.at(constants::HEIGHT_FIELD));
        auto colorObj = std::get<std::map<std::string, std::shared_ptr<FieldValue>>>
            (*fields.at(constants::COLOR_FIELD));
        auto r = static_cast<uint8_t>(std::get<int>(*colorObj.at(constants::R_FIELD)));
        auto g = static_cast<uint8_t>(std::get<int>(*colorObj.at(constants::G_FIELD)));
        auto b = static_cast<uint8_t>(std::get<int>(*colorObj.at(constants::B_FIELD)));
        gfx::color_t color = {r, g, b};
        return std::make_shared<ecs::RectangleRenderComponent>(color, width, height);
    });

    registerComponent<ecs::TextComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        auto text = std::get<std::string>(*fields.at(constants::TEXT_FIELD));
        auto fontPath = std::get<std::string>(*fields.at(constants::FONTPATH_FIELD));
        auto colorObj = std::get<std::map<std::string, std::shared_ptr<FieldValue>>>
            (*fields.at(constants::COLOR_FIELD));
        auto r = static_cast<uint8_t>(std::get<int>(*colorObj.at(constants::R_FIELD)));
        auto g = static_cast<uint8_t>(std::get<int>(*colorObj.at(constants::G_FIELD)));
        auto b = static_cast<uint8_t>(std::get<int>(*colorObj.at(constants::B_FIELD)));
        gfx::color_t color = {r, g, b};
        return std::make_shared<ecs::TextComponent>(text, fontPath, color);
    });

    registerComponent<ecs::ProjectilePrefabComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        auto prefabName = std::get<std::string>(*fields.at(constants::PREFABNAME_FIELD));
        return std::make_shared<ecs::ProjectilePrefabComponent>(prefabName);
    });

    registerComponent<ecs::LifetimeComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        auto lifetime = std::get<float>(*fields.at(constants::LIFETIME_FIELD));
        return std::make_shared<ecs::LifetimeComponent>(lifetime);
    });
    registerComponent<ecs::ScoreTag>([]([[maybe_unused]] const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        return std::make_shared<ecs::ScoreTag>();
    });
    registerComponent<ecs::ScoreComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        auto score = std::get<int>(*fields.at(constants::SCORE_FIELD));
        return std::make_shared<ecs::ScoreComponent>(score);
    });
    registerComponent<ecs::ParallaxComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        auto parallax = std::make_shared<ecs::ParallaxComponent>();

        auto baseSpeed = std::get<float>(*fields.at(constants::BASESCROLLSPEED_FIELD));
        auto direction = std::get<math::Vector2f>(*fields.at(constants::DIRECTION_FIELD));
        auto layersJson = std::get<nlohmann::json>(*fields.at(constants::LAYERS_FIELD));

        parallax->setBaseScrollSpeed(baseSpeed);
        parallax->setDirection(direction);

        for (const auto& layerJson : layersJson) {
            ecs::ParallaxLayer layer;

            layer.name = layerJson.value(constants::NAME_FIELD, "");
            layer.filePath = layerJson.value(constants::FILEPATH_FIELD, "");
            layer.speedMultiplier = layerJson.value(constants::SPEEDMULTIPLIER_FIELD, 1.0f);

            if (layerJson.contains(constants::SCALE_FIELD)) {
                auto scaleJson = layerJson[constants::SCALE_FIELD];
                layer.scale = math::Vector2f(
                    scaleJson.value(constants::X_FIELD, 1.0f),
                    scaleJson.value(constants::Y_FIELD, 1.0f)
                );
            }

            if (layerJson.contains(constants::SCALEMODE_FIELD)) {
                std::string scaleModeStr = layerJson[constants::SCALEMODE_FIELD];
                if (scaleModeStr == constants::SCALEMODE_FITSCREEN) {
                    layer.scaleMode = ecs::ParallaxScaleMode::FIT_SCREEN;
                } else if (scaleModeStr == constants::SCALEMODE_STRETCH) {
                    layer.scaleMode = ecs::ParallaxScaleMode::STRETCH;
                } else if (scaleModeStr == constants::SCALEMODE_MANUAL) {
                    layer.scaleMode = ecs::ParallaxScaleMode::MANUAL;
                } else {
                    layer.scaleMode = ecs::ParallaxScaleMode::FIT_SCREEN;
                }
            }

            if (layerJson.contains(constants::SOURCESIZE_FIELD)) {
                auto sourceSizeJson = layerJson[constants::SOURCESIZE_FIELD];
                layer.sourceSize = math::Vector2f(
                    sourceSizeJson.value(constants::X_FIELD, constants::DEFAULT_TEXTURE_WIDTH),
                    sourceSizeJson.value(constants::Y_FIELD, constants::DEFAULT_TEXTURE_HEIGHT)
                );
            }

            layer.repeat = layerJson.value(constants::REPEAT_FIELD, true);
            layer.zIndex = layerJson.value(constants::ZINDEX_FIELD, 0);
            layer.currentOffset = math::Vector2f(0.0f, 0.0f);

            parallax->addLayer(layer);
        }

        parallax->sortLayersByZIndex();
        return parallax;
    });
    registerComponent<ecs::BackGroundMusicTag>([]([[maybe_unused]]
        const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        return std::make_shared<ecs::BackGroundMusicTag>();
    });
    registerComponent<ecs::MusicComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        auto musicFile = std::get<std::string>(*fields.at(constants::MUSICFILE_FIELD));
        auto initialStateStr = std::get<std::string>
            (*fields.at(constants::INITIALSTATEMUSIC_FIELD));
        auto volume = std::get<float>(*fields.at(constants::VOLUME_FIELD));
        auto loop = std::get<bool>(*fields.at(constants::LOOP_FIELD));
        ecs::MusicState initialState = ecs::STOPPED;
        if (initialStateStr == constants::PLAYING_FIELD) initialState = ecs::PLAYING;
        else if (initialStateStr == constants::PAUSED_FIELD) initialState = ecs::PAUSED;
        else if (initialStateStr == constants::CHANGING_FIELD) initialState = ecs::CHANGING;
        else if (initialStateStr == constants::STOPPED_FIELD) initialState = ecs::STOPPED;
        return std::make_shared<ecs::MusicComponent>(musicFile, initialState, volume, loop);
    });
}

template<typename T>
void Parser::registerComponent(const ComponentCreator& creator) {
    std::type_index idx(typeid(T));
    _componentCreators[idx] = creator;
    _componentAdders[idx] = [](const std::shared_ptr<ecs::Registry>& registry,
        ecs::Entity entity, std::shared_ptr<ecs::IComponent> component) {
        auto originalComponent = std::static_pointer_cast<T>(component);
        auto clonedComponent = std::make_shared<T>(*originalComponent);
        registry->addComponent(entity, clonedComponent);
    };
}
