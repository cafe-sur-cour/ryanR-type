/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** InputMappingManager
*/

#ifndef INPUTMAPPINGMANAGER_HPP_
#define INPUTMAPPINGMANAGER_HPP_

#include <string>
#include "InputMapping.hpp"

namespace ecs {

class InputMappingManager {
public:
    InputMappingManager();
    ~InputMappingManager() = default;

    void loadDefault();

    void setMapping(const InputMapping& mapping);
    const InputMapping& getMapping() const;

private:
    InputMapping _mapping;
};

}  // namespace ecs

#endif /* !INPUTMAPPINGMANAGER_HPP_ */
