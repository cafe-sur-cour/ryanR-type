/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** InputMappingManager
*/

#ifndef INPUTMAPPINGMANAGER_HPP_
#define INPUTMAPPINGMANAGER_HPP_

#include <string>
#include <vector>
#include "InputMapping.hpp"

namespace ecs {

class InputMappingManager {
public:
    InputMappingManager();
    ~InputMappingManager() = default;

    void loadDefault();

    void setMapping(const InputMapping& mapping);
    const InputMapping& getMapping() const;
    InputMapping& getMutableMapping();

    gfx::EventType getKeyboardKeyForAction(InputAction action) const;

    gfx::EventType getKeyboardKeyForActionDirection(InputAction action, float direction) const;

    void remapKeyboardKey(InputAction action, gfx::EventType oldKey, gfx::EventType newKey);

    std::vector<gfx::EventType> getKeyboardKeysForAction(InputAction action) const;

    static std::string eventTypeToString(gfx::EventType eventType);

    static bool isKeyboardKey(gfx::EventType eventType);

private:
    InputMapping _mapping;
};

}  // namespace ecs

#endif /* !INPUTMAPPINGMANAGER_HPP_ */
