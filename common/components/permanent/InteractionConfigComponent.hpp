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
#include "../base/IComponent.hpp"

namespace ecs {

struct InteractionMapping {
    std::vector<std::string> targetTags;
    std::vector<std::string> actionsToOther;
    std::vector<std::string> actionsToSelf;
};

class InteractionConfigComponent : public IComponent {
    public:
        InteractionConfigComponent(){
            _mappings = std::vector<InteractionMapping>();
        };
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
