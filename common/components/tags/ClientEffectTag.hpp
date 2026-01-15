/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ClientEffectTag
*/

#ifndef CLIENTEFFECTTAG_HPP_
#define CLIENTEFFECTTAG_HPP_

#include "../base/IComponent.hpp"

namespace ecs {

class ClientEffectTag : public IComponent {
    public:
        ClientEffectTag() = default;
        ~ClientEffectTag() = default;
};

}

#endif /* !CLIENTEFFECTTAG_HPP_ */
