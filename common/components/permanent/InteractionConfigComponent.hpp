/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** InteractionConfigComponent
*/

#ifndef INTERACTIONCONFIGCOMPONENT_HPP_
#define INTERACTIONCONFIGCOMPONENT_HPP_

#include <vector>
#include <string>
#include "../base/AComponent.hpp"

namespace ecs {

struct InteractionMapping {
    std::vector<std::string> targetTags;
    std::string actionToOther;
    std::string actionToSelf;
};

class InteractionConfigComponent : public AComponent {
    public:
        InteractionConfigComponent() = default;
        InteractionConfigComponent(const std::vector<InteractionMapping>& mappings)
            : _mappings(mappings) {}
        ~InteractionConfigComponent() = default;

        const std::vector<InteractionMapping>& getMappings() const { return _mappings; }
        void setMappings(const std::vector<InteractionMapping>& mappings) { _mappings = mappings; }
        void addMapping(const InteractionMapping& mapping) { _mappings.push_back(mapping); }

    private:
        std::vector<InteractionMapping> _mappings;
};

} // namespace ecs

#endif /* !INTERACTIONCONFIGCOMPONENT_HPP_ */