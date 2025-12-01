/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** View
*/

#ifndef VIEW_HPP_
#define VIEW_HPP_

#include <vector>
#include <memory>
#include <type_traits>

namespace ecs {

template <typename... Components>
class View {
    public:
        View(std::shared_ptr<class Registry> registry);

        class Iterator;

        Iterator begin();
        Iterator end();

        class Iterator {
            public:
                Iterator(std::shared_ptr<class Registry> registry, size_t entityId, size_t maxEntityId);
                bool operator!=(const Iterator& other) const;
                Iterator& operator++();
                size_t operator*() const;

            private:
                bool hasAllComponents() const;
                std::shared_ptr<class Registry> _registry;
                size_t _entityId;
                size_t _maxEntityId;
        };

    private:
        std::shared_ptr<class Registry> _registry;
};

template <typename... Components>
class Group {
    public:
        Group(std::shared_ptr<class Registry> registry);

        class Iterator;

        Iterator begin();
        Iterator end();

        class Iterator {
            public:
                Iterator(std::shared_ptr<class Registry> registry, size_t entityId, size_t maxEntityId);
                bool operator!=(const Iterator& other) const;
                Iterator& operator++();
                size_t operator*() const;

            private:
                bool hasAllComponents() const;
                std::shared_ptr<class Registry> _registry;
                size_t _entityId;
                size_t _maxEntityId;
        };

    private:
        std::shared_ptr<class Registry> _registry;
};

}  // namespace ecs

#include "View.tpp"

#endif /* !VIEW_HPP_ */
