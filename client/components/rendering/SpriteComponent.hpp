/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SpriteComponent
*/

#ifndef SPRITECOMPONENT_HPP_
#define SPRITECOMPONENT_HPP_

#include "../../../common/components/base/IComponent.hpp"
#include "../../../common/types/FRect.hpp"
#include <string>

namespace ecs {

class SpriteComponent : public IComponent {
    public:
        SpriteComponent() : _texturePath("") {}
        SpriteComponent(const std::string& texturePath)
            : _texturePath(texturePath) {}

        ~SpriteComponent() = default;
        const std::string& getTexturePath() const { return _texturePath; }
        void setTexturePath(const std::string& path) { _texturePath = path; }
        bool isValid() const { return !_texturePath.empty(); }

    private:
        std::string _texturePath;
};

} // namespace ecs

#endif /* !SPRITECOMPONENT_HPP_ */
