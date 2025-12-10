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

struct InputMapping {
    std::map<InputAction, std::pair<gfx::EventType, gfx::EventType>> remappableKeys;
    std::map<InputAction, std::map<gfx::EventType, float>> fixedMappings;

    std::map<InputAction, std::map<gfx::EventType, float>> getAllMappings() const {
        std::map<InputAction, std::map<gfx::EventType, float>> all = fixedMappings;
        for (const auto& [action, pair] : remappableKeys) {
            if (pair.first != gfx::EventType::NOTHING) {
                all[action][pair.first] = 1.0f;
            }
            if (pair.second != gfx::EventType::NOTHING) {
                all[action][pair.second] = 1.0f;
            }
        }
        return all;
    }
};

}  // namespace ecs

#endif /* !INPUTMAPPING_HPP_ */
