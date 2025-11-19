/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ASystem
*/

#ifndef ASystem_HPP_
#define ASystem_HPP_

#include "ISystem.hpp"

class ASystem : public ISystem {
    public:
        ASystem();
        ~ASystem() = default;
        void updateSystem(float deltaTime); // window to add

    protected:
    private:
};

#endif /* !ASystem_HPP_ */
