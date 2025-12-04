/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** TagRegistry
*/

#ifndef TAGREGISTRY_HPP_
#define TAGREGISTRY_HPP_

#include <unordered_map>
#include <functional>
#include <memory>
#include <string>
#include "../../ECS/entity/Entity.hpp"
#include "../../ECS/entity/registry/Registry.hpp"

class TagRegistry {
    public:
        static const TagRegistry& getInstance();

        template<typename T>
        void registerTag(const std::string& tagName) {
            _tagCheckers[tagName] = [](std::shared_ptr<ecs::Registry> reg, ecs::Entity ent) {
                return reg->hasComponent<T>(ent);
            };
        }

        bool hasTag(std::shared_ptr<ecs::Registry> registry, ecs::Entity entity, const std::string& tagName) const;

    private:
        TagRegistry();
        ~TagRegistry() = default;
        TagRegistry(const TagRegistry&) = delete;
        TagRegistry& operator=(const TagRegistry&) = delete;

        void initializeTags();

        std::unordered_map<std::string,
            std::function<bool(std::shared_ptr<ecs::Registry>, ecs::Entity)>> _tagCheckers;
};

#endif /* !TAGREGISTRY_HPP_ */
