/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** InteractionConfigComponent registration
*/

#include "InteractionConfigComponent.hpp"
#include "../../Parser/ComponentRegistry/ComponentRegistrar.hpp"
#include "../../constants.hpp"
#include <nlohmann/json.hpp>

REGISTER_COMPONENT(
    ecs::InteractionConfigComponent,
    constants::INTERACTIONCONFIGCOMPONENT,
    {
        Field{constants::TARGET_FIELD, FieldType::STRING},
        Field{constants::MAPPINGS_FIELD, FieldType::JSON}
    },
    [](const std::map<std::string, std::shared_ptr<FieldValue>>& fields) {
        auto mappingsJson = std::get<nlohmann::json>(*fields.at(constants::MAPPINGS_FIELD));
        std::vector<ecs::InteractionMapping> mappings;

        for (const auto& interaction : mappingsJson) {
            ecs::InteractionMapping mapping;
            if (interaction.contains(constants::TAGS_FIELD)) {
                for (const auto& tag : interaction[constants::TAGS_FIELD]) {
                    mapping.targetTags.push_back(tag);
                }
            }
            if (interaction.contains(constants::TOENTITY_FIELD)) {
                auto toEntityValue = interaction[constants::TOENTITY_FIELD];
                if (toEntityValue.is_array()) {
                    for (const auto& action : toEntityValue) {
                        mapping.actionsToOther.push_back(action);
                    }
                } else if (toEntityValue.is_string()) {
                    mapping.actionsToOther.push_back(toEntityValue);
                }
            }
            if (interaction.contains(constants::TOSELF_FIELD)) {
                auto toSelfValue = interaction[constants::TOSELF_FIELD];
                if (toSelfValue.is_array()) {
                    for (const auto& action : toSelfValue) {
                        mapping.actionsToSelf.push_back(action);
                    }
                } else if (toSelfValue.is_string()) {
                    mapping.actionsToSelf.push_back(toSelfValue);
                }
            }
            mappings.push_back(mapping);
        }
        return std::make_shared<ecs::InteractionConfigComponent>(mappings);
    }
)
