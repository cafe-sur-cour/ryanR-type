#include "ComponentRegistry.hpp"
#include <stdexcept>

namespace parser {

ComponentRegistry& ComponentRegistry::getInstance() {
    static ComponentRegistry instance;
    return instance;
}

void ComponentRegistry::registerComponent(const ComponentMetadata& metadata) {
    _components.insert({metadata.name, metadata});
}

bool ComponentRegistry::hasComponent(const std::string& name) const {
    return _components.find(name) != _components.end();
}

const ComponentMetadata& ComponentRegistry::getMetadata(const std::string& name) const {
    auto it = _components.find(name);
    if (it == _components.end()) {
        throw std::runtime_error("Component '" + name + "' not found in registry");
    }
    return it->second;
}

std::type_index ComponentRegistry::getTypeIndex(const std::string& name) const {
    return getMetadata(name).typeIndex;
}

const std::map<std::string, ComponentMetadata>& ComponentRegistry::getAllComponents() const {
    return _components;
}

std::shared_ptr<std::map<std::string, std::pair<std::type_index, std::vector<Field>>>> 
ComponentRegistry::getComponentDefinitions() const {
    auto definitions = std::make_shared<
        std::map<std::string, std::pair<std::type_index, std::vector<Field>>>
    >();

    for (const auto& [name, metadata] : _components) {
        definitions->insert({name, std::make_pair(metadata.typeIndex, metadata.fields)});
    }
    
    return definitions;
}

std::map<std::type_index, ComponentCreator> ComponentRegistry::getComponentCreators() const {
    std::map<std::type_index, ComponentCreator> creators;

    for (const auto& [name, metadata] : _components) {
        creators[metadata.typeIndex] = metadata.creator;
    }

    return creators;
}

std::map<std::type_index, ComponentAdder> ComponentRegistry::getComponentAdders() const {
    std::map<std::type_index, ComponentAdder> adders;

    for (const auto& [name, metadata] : _components) {
        adders[metadata.typeIndex] = metadata.adder;
    }

    return adders;
}

}  // namespace parser
