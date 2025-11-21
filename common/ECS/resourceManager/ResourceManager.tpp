/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ResourceManager
*/

#include "ResourceManager.hpp"

namespace ecs {

template<typename T>
void ResourceManager::add(std::shared_ptr<T> resource) {
    resources[typeid(T).hash_code()] = resource;
    }

template<typename T>
std::shared_ptr<T> ResourceManager::get() {
    return std::static_pointer_cast<T>(resources[typeid(T).hash_code()]);
}

template<typename T>
bool ResourceManager::has() {
    return resources.find(typeid(T).hash_code()) != resources.end();
}

} // namespace ecs