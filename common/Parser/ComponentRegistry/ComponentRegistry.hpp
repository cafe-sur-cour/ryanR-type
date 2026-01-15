#ifndef COMPONENTREGISTRY_HPP_
#define COMPONENTREGISTRY_HPP_

#include <map>
#include <string>
#include <memory>
#include <typeindex>
#include "ComponentMetadata.hpp"

namespace parser {

class ComponentRegistry {
public:
    static ComponentRegistry& getInstance();

    void registerComponent(const ComponentMetadata &metadata);

    bool hasComponent(const std::string& name) const;

    const std::map<std::string, ComponentMetadata> &getAllComponents() const;

    std::shared_ptr<std::map<std::string, std::pair<std::type_index, std::vector<Field>>>> getComponentDefinitions() const;
    std::map<std::type_index, ComponentCreator> getComponentCreators() const;
    std::map<std::type_index, ComponentAdder> getComponentAdders() const;

private:
    ComponentRegistry() = default;
    ComponentRegistry(const ComponentRegistry&) = delete;
    ComponentRegistry& operator=(const ComponentRegistry&) = delete;

    std::map<std::string, ComponentMetadata> _components;
};

}  // namespace parser

#endif  // COMPONENTREGISTRY_HPP_
