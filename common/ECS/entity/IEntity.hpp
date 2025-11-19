/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** IEntity
*/

#ifndef IENTITY_HPP_
#define IENTITY_HPP_

#include <cstddef>

class IEntity {
    public:
        IEntity();
        ~IEntity();
        virtual operator size_t() const = 0;

    private:
        explicit IEntity(size_t id);
};
#endif /* !IENTITY_HPP_ */
