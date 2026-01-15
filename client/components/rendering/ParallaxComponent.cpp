/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ParallaxComponent
*/

#include "ParallaxComponent.hpp"
#include <map>
#include <string>
#include <memory>
#include <nlohmann/json.hpp>
#include "../../../common/Parser/ComponentRegistry/ComponentRegistrar.hpp"
#include "../../../common/constants.hpp"

REGISTER_COMPONENT(
    ecs::ParallaxComponent,
    constants::PARALLAXCOMPONENT,
    {
        Field{constants::TARGET_FIELD, FieldType::STRING},
        Field{constants::BASESCROLLSPEED_FIELD, FieldType::FLOAT},
        Field{constants::DIRECTION_FIELD, FieldType::VECTOR2F},
        Field{constants::LAYERS_FIELD, FieldType::JSON}
    },
    [](const std::map<std::string, std::shared_ptr<FieldValue>>& fields) {
        auto parallax = std::make_shared<ecs::ParallaxComponent>();

        auto baseSpeed = std::get<float>(*fields.at(constants::BASESCROLLSPEED_FIELD));
        auto direction = std::get<math::Vector2f>(*fields.at(constants::DIRECTION_FIELD));
        auto layersJson = std::get<nlohmann::json>(*fields.at(constants::LAYERS_FIELD));

        parallax->setBaseScrollSpeed(baseSpeed);
        parallax->setDirection(direction);

        for (const auto& layerJson : layersJson) {
            std::shared_ptr<ecs::ParallaxLayer> layer = std::make_shared<ecs::ParallaxLayer>();

            layer->name = layerJson.value(constants::NAME_FIELD, "");
            layer->filePath = layerJson.value(constants::FILEPATH_FIELD, "");
            layer->speedMultiplier = layerJson.value(constants::SPEEDMULTIPLIER_FIELD, 1.0f);

            if (layerJson.contains(constants::SCALE_FIELD)) {
                auto scaleJson = layerJson[constants::SCALE_FIELD];
                layer->scale = math::Vector2f(
                    scaleJson.value(constants::X_FIELD, 1.0f),
                    scaleJson.value(constants::Y_FIELD, 1.0f)
                );
            }

            if (layerJson.contains(constants::SCALEMODE_FIELD)) {
                std::string scaleModeStr = layerJson[constants::SCALEMODE_FIELD];
                if (scaleModeStr == constants::SCALEMODE_FITSCREEN) {
                    layer->scaleMode = ecs::ParallaxScaleMode::FIT_SCREEN;
                } else if (scaleModeStr == constants::SCALEMODE_STRETCH) {
                    layer->scaleMode = ecs::ParallaxScaleMode::STRETCH;
                } else if (scaleModeStr == constants::SCALEMODE_MANUAL) {
                    layer->scaleMode = ecs::ParallaxScaleMode::MANUAL;
                } else {
                    layer->scaleMode = ecs::ParallaxScaleMode::FIT_SCREEN;
                }
            }

            if (layerJson.contains(constants::SOURCESIZE_FIELD)) {
                auto sourceSizeJson = layerJson[constants::SOURCESIZE_FIELD];
                layer->sourceSize = math::Vector2f(
                    sourceSizeJson.value(constants::X_FIELD, constants::DEFAULT_TEXTURE_WIDTH),
                    sourceSizeJson.value(constants::Y_FIELD, constants::DEFAULT_TEXTURE_HEIGHT)
                );
            }

            layer->repeat = layerJson.value(constants::REPEAT_FIELD, true);
            layer->zIndex = layerJson.value(constants::ZINDEX_FIELD, 0);
            layer->currentOffset = math::Vector2f(0.0f, 0.0f);

            parallax->addLayer(layer);
        }

        auto layers = parallax->getLayers();

        std::sort(
            layers.begin(),
            layers.end(),
            [](
                const std::shared_ptr<ecs::ParallaxLayer>& a,
                const std::shared_ptr<ecs::ParallaxLayer>& b
            ) {
                return a->zIndex < b->zIndex;
            }
        );
        return parallax;
    }
)
