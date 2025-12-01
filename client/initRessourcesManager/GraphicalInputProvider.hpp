/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** GraphicalInputProvider
*/

#include "../../common/ECS/resourceManager/IInputProvider.hpp"
#include "../../libs/Multimedia/IEvent.hpp"
#include "../../common/ECS/resourceManager/InputMapping.hpp"
#include <memory>

namespace ecs {

class GraphicalInputProvider : public IInputProvider {
    public:
        GraphicalInputProvider(std::shared_ptr<gfx::IEvent> eventSystem, const InputMapping& mapping);
        ~GraphicalInputProvider() override = default;

        float getAxisValue(event_t axis) override;

        bool isActionPressed(InputAction action) override;
        float getActionAxis(InputAction action) override;

    private:
        std::shared_ptr<gfx::IEvent> _eventSystem;
        InputMapping _mapping;
};

} // namespace ecs
