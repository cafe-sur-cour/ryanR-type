/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** NetworkIdComponent
*/

#ifndef NETWORKIDCOMPONENT_HPP_
#define NETWORKIDCOMPONENT_HPP_

#include "../base/AComponent.hpp"
#include <cstddef>

namespace ecs {

class NetworkIdComponent : public AComponent {
    public:
        explicit NetworkIdComponent(size_t networkId = 0)
            : _networkId(networkId) {}

        ~NetworkIdComponent() = default;

        size_t getNetworkId() const { return _networkId; }
        void setNetworkId(size_t networkId) { _networkId = networkId; }

    private:
        size_t _networkId;
};

} // namespace ecs

#endif /* !NETWORKIDCOMPONENT_HPP_ */
