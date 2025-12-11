/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** InputMapping
*/

#ifndef INPUTMAPPING_HPP_
#define INPUTMAPPING_HPP_

#include <map>
#include <vector>
#include "../../libs/Multimedia/EventTypes.hpp"
#include "InputAction.hpp"

namespace ecs {

struct RemappableKeyBinding {
    gfx::EventType primary;
    gfx::EventType secondary;

    RemappableKeyBinding()
        : primary(gfx::EventType::NOTHING), secondary(gfx::EventType::NOTHING) {}
    RemappableKeyBinding(gfx::EventType p, gfx::EventType s)
        : primary(p), secondary(s) {}
};

struct InputMapping {
    std::map<RemappableAction, RemappableKeyBinding> remappableKeys;
    std::map<InputAction, std::map<gfx::EventType, float>> fixedMappings;

    std::map<InputAction, std::map<gfx::EventType, float>> getAllMappings() const {
        std::map<InputAction, std::map<gfx::EventType, float>> all = fixedMappings;

        for (const auto& [action, binding] : remappableKeys) {
            InputAction inputAction;
            switch (action) {
                case RemappableAction::MOVE_LEFT: inputAction = InputAction::MOVE_X; break;
                case RemappableAction::MOVE_RIGHT: inputAction = InputAction::MOVE_X; break;
                case RemappableAction::MOVE_UP: inputAction = InputAction::MOVE_Y; break;
                case RemappableAction::MOVE_DOWN: inputAction = InputAction::MOVE_Y; break;
                case RemappableAction::SHOOT: inputAction = InputAction::SHOOT; break;
            }

            float value = (action == RemappableAction::MOVE_LEFT ||
                action == RemappableAction::MOVE_UP) ? -1.0f : 1.0f;

            if (binding.primary != gfx::EventType::NOTHING) {
                all[inputAction][binding.primary] = value;
            }
            if (binding.secondary != gfx::EventType::NOTHING) {
                all[inputAction][binding.secondary] = value;
            }
        }

        return all;
    }
};

}  // namespace ecs

#endif /* !INPUTMAPPING_HPP_ */
