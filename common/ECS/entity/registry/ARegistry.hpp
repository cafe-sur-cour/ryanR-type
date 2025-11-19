/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ARegistry
*/

#ifndef AREGISTRY_HPP_
#define AREGISTRY_HPP_

#include "IRegistry.hpp"
#include "../../component/IComponent.hpp"
#include <memory>
#include <unordered_map>

class ARegistry : public IRegistry {
    public:
        ARegistry();
        virtual ~ARegistry();

        template <typename T>
        void registerComponent();

        template <typename T>
        void addComponent(int entityId, std::shared_ptr<T> component);
        template <typename T>
        std::shared_ptr<T> getComponent(int entityId);
        template <typename T>
        void removeComponent(int entityId);
        template <typename T>
        bool hasComponent(int entityId);
    protected:
    private:
        std::unordered_map<const char *, std::shared_ptr<IComponent>> _components; //the char * is typeid(T).name()
};

#endif /* !AREGISTRY_HPP_ */
