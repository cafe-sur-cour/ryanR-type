/*
** EPITECH PROJECT, 2026
** ryanR-type
** File description:
** ScriptingSystem
*/

#ifndef SCRIPTINGSYSTEM_HPP_
#define SCRIPTINGSYSTEM_HPP_

#include "../base/ASystem.hpp"
#include "../../components/permanent/ScriptingComponent.hpp"
#include "../../components/permanent/TransformComponent.hpp"
#include "../../components/permanent/SpeedComponent.hpp"
#include "../../components/tags/LocalPlayerTag.hpp"

#include <sol/sol.hpp>

namespace ecs {

class ScriptingSystem : public ASystem {
    public:
        ScriptingSystem();
        ~ScriptingSystem() = default;

        void update(
            std::shared_ptr<ResourceManager> resourceManager,
            std::shared_ptr<Registry> reg,
            float deltaTime
        ) override;

    protected:
    private:
        void bindAPI();
        void callInitForEntity(Entity entity, std::shared_ptr<ecs::ScriptingComponent> comp);

        sol::state lua;
        std::shared_ptr<Registry> registry;
};

}  // namespace ecs

#endif /* !SCRIPTINGSYSTEM_HPP_ */
