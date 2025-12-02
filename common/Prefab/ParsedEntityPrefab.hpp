/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ParsedEntityPrefab
*/

#ifndef PARSEDENTITYPREFAB_HPP_
#define PARSEDENTITYPREFAB_HPP_

#include "IPrefab.hpp"
#include <vector>
#include <memory>
#include <string>
#include <typeindex>
#include <map>
#include <functional>
#include "../components/base/IComponent.hpp"
#include "../ECS/entity/registry/Registry.hpp"
#include "../Parser/ParserParam.hpp"

class ParsedEntityPrefab : public IPrefab {
    public:
        ParsedEntityPrefab(const std::string& name, const std::map<std::type_index, ComponentAdder>& adders);
        ~ParsedEntityPrefab();

        void addComponent(std::shared_ptr<ecs::IComponent> component, std::type_index typeIndex);
        const std::vector<std::shared_ptr<ecs::IComponent>>& getComponents() const;
        std::string getName() const;

        ecs::Entity instantiate(const std::shared_ptr<ecs::Registry>& registry) override;

    private:
        std::string _name;
        std::vector<std::pair<std::shared_ptr<ecs::IComponent>, std::type_index>> _components;
        const std::map<std::type_index, ComponentAdder>& _componentAdders;
};

#endif /* !PARSEDENTITYPREFAB_HPP_ */