/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ASystem
*/

#ifndef ASystem_HPP_
#define ASystem_HPP_

#include "ISystem.hpp"
#include "../context/AContext.hpp"
#include <memory>


class ASystem : public ISystem {
    public:
        ASystem();
        ~ASystem() = default;
        void updateSystem(std::shared_ptr<AContext> context, float deltaTime) override;

    protected:
        virtual void update(std::shared_ptr<AContext> context, float deltaTime) = 0;

    private:
};

#endif /* !ASystem_HPP_ */
