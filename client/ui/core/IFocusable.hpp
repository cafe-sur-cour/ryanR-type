/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** IFocusable
*/

#ifndef IFOCUSABLE_HPP_
#define IFOCUSABLE_HPP_

#include <memory>

namespace ui {

class IFocusable {
    public:
        virtual ~IFocusable() = default;

        virtual void setFocused(bool focused) = 0;
        virtual bool isFocused() const = 0;
        virtual bool canBeFocused() const = 0;

        virtual void onFocusGained() = 0;
        virtual void onFocusLost() = 0;
        virtual void onActivated() = 0;

        virtual bool onNavigateLeft() { return false; }
        virtual bool onNavigateRight() { return false; }
};

}  // namespace ui

#endif /* !IFOCUSABLE_HPP_ */
