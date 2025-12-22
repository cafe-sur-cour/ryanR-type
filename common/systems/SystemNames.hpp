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

/* Client Systems */
const std::string SPRITE_RENDERING_SYSTEM = "SpriteRenderingSystem";
const std::string ANIMATION_RENDERING_SYSTEM = "AnimationRenderingSystem";
const std::string HITBOX_RENDERING_SYSTEM = "HitboxRenderingSystem";
const std::string RECTANGLE_RENDERING_SYSTEM = "RectangleRenderingSystem";
const std::string TEXT_RENDERING_SYSTEM = "TextRenderingSystem";
const std::string PARALLAX_RENDERING_SYSTEM = "ParallaxRenderingSystem";
const std::string GAME_ZONE_RENDERING_SYSTEM = "GameZoneRenderingSystem";
const std::string GAME_ZONE_VIEW_SYSTEM = "GameZoneViewSystem";
const std::string HEALTH_BAR_RENDERING_SYSTEM = "HealthBarRenderingSystem";
const std::string SOUND_SYSTEM = "SoundSystem";
const std::string MUSIC_SYSTEM = "MusicSystem";
const std::string CLIENT_EFFECT_CLEANUP_SYSTEM = "ClientEffectCleanupSystem";
const std::string MOVEMENT_INPUT_SYSTEM = "MovementInputSystem";
const std::string SHOOT_INPUT_SYSTEM = "ShootInputSystem";
const std::string NETWORK_INTERPOLATION_SYSTEM = "NetworkInterpolationSystem";

/* Common Systems */
const std::string DEATH_SYSTEM = "DeathSystem";
const std::string AI_MOVEMENT_SYSTEM = "AIMovementSystem";
const std::string AI_SHOOTING_SYSTEM = "AIShootingSystem";
const std::string HEALTH_SYSTEM = "HealthSystem";
const std::string OUT_OF_BOUNDS_SYSTEM = "OutOfBoundsSystem";
const std::string SPAWN_SYSTEM = "SpawnSystem";
const std::string SHOOTING_SYSTEM = "ShootingSystem";
const std::string SCORE_SYSTEM = "ScoreSystem";
const std::string LIFETIME_SYSTEM = "LifetimeSystem";
const std::string MOVEMENT_SYSTEM = "MovementSystem";
const std::string INPUT_TO_VELOCITY_SYSTEM = "InputToVelocitySystem";
const std::string TRIGGER_SYSTEM = "TriggerSystem";
const std::string INTERACTION_SYSTEM = "InteractionSystem";

}  // namespace ecs::systems

#endif /* !SYSTEMNAMES_HPP_ */
