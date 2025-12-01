/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AComposantType
*/

#ifndef AComposantType_HPP_
#define AComposantType_HPP_

#include "IComponentArray.hpp"
#include "../../../components/base/IComponent.hpp"
#include <vector>
#include <memory>

namespace ecs {

template <typename T>
class AComponentArray : public IComponentArray {
    public:
        AComponentArray();
        ~AComponentArray() override;

        void add(Entity entityId, std::shared_ptr<T> component);
        std::shared_ptr<T> get(Entity entityId) const;
        std::vector<std::shared_ptr<T>> getAll(Entity entityId) const;
        void remove(Entity entityId);
        bool has(Entity entityId) const;

        void removeAllComponentsWithState(ComponentState state) override;
        Entity getMaxEntityId() const override;

    private:
        std::vector<std::vector<std::shared_ptr<T>>> _components;
};

} // namespace ecs

#include "AComponentArray.tpp"

#endif /* !AComposantType_HPP_ */
