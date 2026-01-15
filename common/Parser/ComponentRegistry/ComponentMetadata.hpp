#ifndef COMPONENTMETADATA_HPP_
#define COMPONENTMETADATA_HPP_

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <typeindex>
#include "../ParserParam.hpp"
#include "../../components/base/IComponent.hpp"
#include "../../ECS/entity/Entity.hpp"
#include "../../ECS/entity/registry/Registry.hpp"

namespace parser {

using ComponentCreator = std::function<std::shared_ptr<ecs::IComponent>(
    const std::map<std::string, std::shared_ptr<FieldValue>> &
)>;

using ComponentAdder = std::function<void(
    std::shared_ptr<ecs::Registry>, ecs::Entity, std::shared_ptr<ecs::IComponent>
)>;

struct ComponentMetadata {
    std::string name;                    // Name of JSON component (eg: "TransformComponent")
    std::type_index typeIndex;           // typeid of component
    std::vector<Field> fields;           // List of parsable fields
    ComponentCreator creator;         // Creation lambda
    ComponentAdder adder;             // Adder to registry lambda
};

}  // namespace parser

#endif  // COMPONENTMETADATA_HPP_
