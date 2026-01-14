/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ShootingStatsComponent
*/

#include "ShootingStatsComponent.hpp"
#include <map>
#include <string>
#include <memory>
#include "../../Parser/ComponentRegistry/ComponentRegistrar.hpp"
#include "../../constants.hpp"

REGISTER_COMPONENT(
    ecs::ShootingStatsComponent,
    constants::SHOOTINGSTATSCOMPONENT,
    {
        Field{constants::TARGET_FIELD, FieldType::STRING},
        Field{constants::FIRERATE_FIELD, FieldType::FLOAT},
        Field{constants::SHOTCOUNT_FIELD, FieldType::INT, true,
              std::make_shared<FieldValue>(1)},
        Field{constants::ANGLEOFFSET_FIELD, FieldType::FLOAT, true,
              std::make_shared<FieldValue>(0.0f)},
        Field{constants::SPREADANGLE_FIELD, FieldType::FLOAT, true,
              std::make_shared<FieldValue>(0.0f)},
        Field{constants::OFFSETDISTANCE_FIELD, FieldType::FLOAT, true,
              std::make_shared<FieldValue>(0.0f)}
    },
    [](const std::map<std::string, std::shared_ptr<FieldValue>>& fields) {
        auto fireRate = std::get<float>(*fields.at(constants::FIRERATE_FIELD));
        auto shotCount = std::get<int>(*fields.at(constants::SHOTCOUNT_FIELD));
        auto angleOffset = std::get<float>(*fields.at(constants::ANGLEOFFSET_FIELD));
        auto spreadAngle = std::get<float>(*fields.at(constants::SPREADANGLE_FIELD));
        auto offsetDistance = std::get<float>(*fields.at(constants::OFFSETDISTANCE_FIELD));

        ecs::MultiShotPattern pattern{
            shotCount,
            spreadAngle,
            offsetDistance,
            angleOffset
        };

        return std::make_shared<ecs::ShootingStatsComponent>(fireRate, pattern);
    }
)
