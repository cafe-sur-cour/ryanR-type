/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AContext
*/

#ifndef ACONTEXT_HPP_
#define ACONTEXT_HPP_

#include "../entity/registry/ARegistry.hpp"
#include "../resourceManager/ResourceManager.hpp"
#include <memory>

class AContext {
    public:
        AContext() : _registry(std::make_shared<ARegistry>()),
                    _resourceManager(std::make_shared<ResourceManager>()) {}

        virtual ~AContext() = default;
        std::shared_ptr<ARegistry> getRegistry() const {
            return _registry;
        }

        std::shared_ptr<ResourceManager> getResourceManager() const {
            return _resourceManager;
        }

        template<typename T>
        void addResource(std::shared_ptr<T> resource) {
            _resourceManager->add(resource);
        }

        template<typename T>
        std::shared_ptr<T> getResource() {
            return _resourceManager->get<T>();
        }

    protected:
        std::shared_ptr<ARegistry> _registry;
        std::shared_ptr<ResourceManager> _resourceManager;
};

#endif /* !ACONTEXT_HPP_ */
