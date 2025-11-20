/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ResourceManager
*/

#ifndef RESOURCEMANAGER_HPP_
#define RESOURCEMANAGER_HPP_

#include <unordered_map>
#include <memory>

class ResourceManager {
    public:
        template<typename T>
        void add(std::shared_ptr<T> resource);

        template<typename T>
        std::shared_ptr<T> get();

        template<typename T>
        bool has();

    private:
        std::unordered_map<size_t, std::shared_ptr<void>> resources;
};

#endif /* !RESOURCEMANAGER_HPP_ */
