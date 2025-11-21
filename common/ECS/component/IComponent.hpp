/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** IComponent
*/

#ifndef ICOMPONENT_HPP_
#define ICOMPONENT_HPP_

namespace ecs {

enum ComponentState {
    Permanent = 0,
    Temporary = 1,
    Processed = 2,
};

class IComponent {
    public:
        IComponent();
        virtual ~IComponent();

        virtual ComponentState getState() const = 0;
        virtual void setState(ComponentState newState) = 0;

    protected:
    private:
};

} // namespace ecs

#endif /* !ICOMPONENT_HPP_ */
