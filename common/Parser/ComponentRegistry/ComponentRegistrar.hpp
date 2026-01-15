#ifndef COMPONENTREGISTRAR_HPP_
#define COMPONENTREGISTRAR_HPP_

#include "ComponentRegistry.hpp"
#include "ComponentMetadata.hpp"

namespace parser {

template<typename T>
class ComponentRegistrar {
    public:
        ComponentRegistrar(
            const std::string& name,
            const std::vector<Field>& fields,
            const ComponentCreator& creator
        ) {
            ComponentMetadata metadata{
                name,
                std::type_index(typeid(T)),
                fields,
                creator,
                [](
                    std::shared_ptr<ecs::Registry> registry,
                    ecs::Entity entity,
                    std::shared_ptr<ecs::IComponent> component
                ) {
                    auto typedComponent = std::static_pointer_cast<T>(component);
                    auto cloned = std::make_shared<T>(*typedComponent);
                    registry->addComponent(entity, cloned);
                }
            };

            ComponentRegistry::getInstance().registerComponent(metadata);
        }
};

template<typename T>
class TagComponentRegistrar {
    public:
        explicit TagComponentRegistrar(const std::string& name) {
            ComponentMetadata metadata{
                name,
                std::type_index(typeid(T)),
                {{"target", FieldType::STRING, false, nullptr}},
                []([[maybe_unused]] const auto& fields) {
                    return std::make_shared<T>();
                },
                [](
                    std::shared_ptr<ecs::Registry> registry,
                    ecs::Entity entity,
                    std::shared_ptr<ecs::IComponent> component
                ) {
                    registry->addComponent(entity, std::static_pointer_cast<T>(component));
                }
            };

            ComponentRegistry::getInstance().registerComponent(metadata);
        }
};

}  // namespace parser

#define REGISTER_COMPONENT(Type, Name, ...) \
    namespace { \
        static parser::ComponentRegistrar<Type> CONCAT(_registrar_, __COUNTER__)(Name, __VA_ARGS__); \
    }

#define REGISTER_TAG_COMPONENT(Type, Name) \
    namespace { \
        static parser::TagComponentRegistrar<Type> CONCAT(_tag_registrar_, __COUNTER__)(Name); \
    }

#define CONCAT_IMPL(x, y) x##y
#define CONCAT(x, y) CONCAT_IMPL(x, y)

#endif  // COMPONENTREGISTRAR_HPP_
