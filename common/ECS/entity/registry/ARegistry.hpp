/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ARegistry
*/

#ifndef AREGISTRY_HPP_
#define AREGISTRY_HPP_

#include "IRegistry.hpp"
#include "../../component/base/IComponent.hpp"
#include "../componentArray/IComponentArray.hpp"
#include "../componentArray/AComponentArray.hpp"
#include "../../view/View.hpp"
#include <memory>
#include <unordered_map>
#include <string>

namespace ecs {

class ARegistry : public IRegistry, public std::enable_shared_from_this<ARegistry> {
    public:
        ARegistry();
        virtual ~ARegistry();

        template <typename T>
        void registerComponent();

        template <typename T>
        void addComponent(int entityId, std::shared_ptr<T> component);
        template <typename T>
        std::shared_ptr<T> getComponent(int entityId) const;
        template <typename T>
        void removeComponent(int entityId);
        template <typename T>
        bool hasComponent(int entityId) const;

        template <typename... Components>
        View<Components...> view();

        template <typename... Components>
        Group<Components...> group();

        size_t getMaxEntityId() const;

        void removeAllComponentsWithState(ComponentState state) override;

    protected:
    private:
        std::unordered_map<std::string, std::shared_ptr<IComponentArray>> _components;
};

} // namespace ecs

#include "ARegistry.tpp"

#endif /* !AREGISTRY_HPP_ */
