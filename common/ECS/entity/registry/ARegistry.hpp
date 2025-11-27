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
        void addComponent(size_t entityId, std::shared_ptr<T> component);
        template <typename T>
        std::shared_ptr<T> getComponent(size_t entityId) const;
        template <typename T>
        std::vector<std::shared_ptr<T>> getComponents(size_t entityId) const;
        template <typename T>
        void removeComponent(size_t entityId);
        template <typename T>
        bool hasComponent(size_t entityId) const;

        template <typename... Components>
        View<Components...> view();

        template <typename... Components>
        Group<Components...> group();

        size_t getMaxEntityId() const;

        void removeAllComponentsWithState(ComponentState state) override;
        size_t createEntity() override;
    protected:
    private:
        size_t _nextEntityId;
        std::unordered_map<std::string, std::shared_ptr<IComponentArray>> _components;
};

} // namespace ecs

#include "ARegistry.tpp"

#endif /* !AREGISTRY_HPP_ */
