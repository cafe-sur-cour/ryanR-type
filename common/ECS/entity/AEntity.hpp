/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** IEntity
*/

#ifndef AENTITY_HPP_
#define AENTITY_HPP_

#include "IEntity.hpp"

namespace ecs {

class AEntity : public IEntity {
    public:
        AEntity();
        ~AEntity();
        operator size_t() const override;

    private:
        explicit AEntity(size_t id);
        size_t _id;
};

}  // namespace ecs

#endif /* !AENTITY_HPP_ */