/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** EndOfMapDetectionSystem
*/

#ifndef ENDOFMAPDETECTIONSYSTEM_HPP_
#define ENDOFMAPDETECTIONSYSTEM_HPP_


#include "../../../common/systems/base/ASystem.hpp"

namespace ecs  {


class EndOfMapDetectionSystem : public ASystem{
    public:
        EndOfMapDetectionSystem();
        ~EndOfMapDetectionSystem();

        void update(std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<Registry> registry, float deltaTime) override;

    protected:
    private:
};

}  // namespace ecs

#endif /* !ENDOFMAPDETECTIONSYSTEM_HPP_ */
