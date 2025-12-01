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
    std::map<InputAction, std::map<gfx::EventType, float>> mappings;
};

}  // namespace ecs

#endif /* !INPUTMAPPING_HPP_ */
