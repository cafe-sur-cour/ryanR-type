/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ServerInputProvider
*/

#include "../../common/InputMapping/IInputProvider.hpp"
#include "../../common/InputMapping/InputMapping.hpp"

namespace ecs {

class ServerInputProvider : public IInputProvider {
    public:
        ServerInputProvider();
        ~ServerInputProvider() override = default;

        float getAxisValue(event_t axis) override;

        bool isActionPressed(InputAction action) override;
        float getActionAxis(InputAction action) override;
        InputMapping getInputMapping() const override;

    private:
        InputMapping _inputMapping;
};

} // namespace ecs
