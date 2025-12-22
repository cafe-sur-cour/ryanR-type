/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SystemNames
*/

#ifndef SYSTEMNAMES_HPP_
#define SYSTEMNAMES_HPP_

#include <string>

namespace ecs::systems {

const std::string DEATH_SYSTEM = "DeathSystem";
const std::string SPRITE_RENDERING_SYSTEM = "SpriteRenderingSystem";
const std::string ANIMATION_RENDERING_SYSTEM = "AnimationRenderingSystem";
const std::string HITBOX_RENDERING_SYSTEM = "HitboxRenderingSystem";
const std::string RECTANGLE_RENDERING_SYSTEM = "RectangleRenderingSystem";
const std::string TEXT_RENDERING_SYSTEM = "TextRenderingSystem";
const std::string PARALLAX_RENDERING_SYSTEM = "ParallaxRenderingSystem";
const std::string GAME_ZONE_RENDERING_SYSTEM = "GameZoneRenderingSystem";
const std::string GAME_ZONE_VIEW_SYSTEM = "GameZoneViewSystem";
const std::string HEALTH_BAR_RENDERING_SYSTEM = "HealthBarRenderingSystem";

}  // namespace ecs::systems

#endif /* !SYSTEMNAMES_HPP_ */
