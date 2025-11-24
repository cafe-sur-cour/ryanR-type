/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AEntity
*/

#include "AEntity.hpp"

namespace ecs {

AEntity::AEntity(): _id(0) {
}

AEntity::~AEntity() {
}

AEntity::operator size_t() const {
    return _id;
}

AEntity::AEntity(size_t id): _id(id) {
}

}  // namespace ecs
