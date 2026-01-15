/*
** EPITECH PROJECT, 2026
** ryanR-type
** File description:
** ForceTag
*/

#ifndef FORCETAG_HPP_
#define FORCETAG_HPP_

#include "../base/IComponent.hpp"
#include <string>

namespace ecs {

class ForceTag : public IComponent {
    public:
        ForceTag(const std::string& type = "") : forceType(type) {}
        ~ForceTag() override = default;

        std::string getForceType() const { return forceType; }
        void setForceType(const std::string& type) { forceType = type; }

    private:
        std::string forceType;
};

}  // namespace ecs

#endif /* !FORCETAG_HPP_ */