/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ISystem
*/

#ifndef ISystem_HPP_
#define ISystem_HPP_

class ISystem {
    public:
        virtual ~ISystem();
        virtual void updateSystem(float deltaTime) = 0; // window to add
};

#endif /* !ISystem_HPP_ */
