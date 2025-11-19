/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AComposantType
*/

#ifndef AComposantType_HPP_
#define AComposantType_HPP_

#include "../component/IComponent.hpp"
#include "IComposantType.hpp"
#include <vector>
#include <memory>


template <typename T>
class AComposantType : public IAComposantType {
    public:
        AComposantType();
        ~AComposantType();

        void add(int entityId, std::shared_ptr<T> component);
        std::shared_ptr<T> get(int entityId);
        void remove(int entityId);
        bool has(int entityId) const;

    private:
        std::vector<std::shared_ptr<T>> _components;
};

#endif /* !AComposantType_HPP_ */
