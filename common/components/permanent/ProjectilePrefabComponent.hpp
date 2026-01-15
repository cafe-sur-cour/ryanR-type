/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ProjectilePrefabComponent
*/

#ifndef PROJECTILEPREFABCOMPONENT_HPP_
#define PROJECTILEPREFABCOMPONENT_HPP_

#include "../base/IComponent.hpp"
#include <string>

namespace ecs {

class ProjectilePrefabComponent : public IComponent {
    public:
        ProjectilePrefabComponent(const std::string &prefabName = "")
            : _prefabName(prefabName) {};
        ~ProjectilePrefabComponent() = default;

        std::string getPrefabName() const { return _prefabName; };
        void setPrefabName(const std::string &prefabName) { _prefabName = prefabName; };

    private:
        std::string _prefabName;
};

}

#endif /* !PROJECTILEPREFABCOMPONENT_HPP_ */
