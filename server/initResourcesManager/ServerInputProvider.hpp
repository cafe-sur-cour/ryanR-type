/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ServerInputProvider
*/

#include "../../common/ImputMapping/IInputProvider.hpp"

namespace ecs {

class ServerInputProvider : public IInputProvider {
    public:
        ServerInputProvider();
        ~ServerInputProvider() override = default;

        float getAxisValue(event_t axis) override;

        bool isActionPressed(InputAction action) override;
        float getActionAxis(InputAction action) override;
};

} // namespace ecs
