/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AComposantType
*/

#ifndef AComposantType_HPP_
#define AComposantType_HPP_

#include "../../component/IComponent.hpp"
#include <vector>
#include <memory>


template <typename T>
class AComponentArray : public IComponentArray {
    public:
        AComponentArray();
        ~AComponentArray();

        void add(int entityId, std::shared_ptr<T> component);
        std::shared_ptr<T> get(int entityId) const;
        void remove(int entityId);
        bool has(int entityId) const;

        void removeAllComponentsWithState(ComponentState state) override;

    private:
        std::vector<std::shared_ptr<T>> _components;
};

#endif /* !AComposantType_HPP_ */
