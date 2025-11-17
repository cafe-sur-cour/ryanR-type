/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** LoaderType
*/

#ifndef LOADERTYPE_HPP_
#define LOADERTYPE_HPP_

enum ModuleType_t{
    DISPLAY_MODULE,
    NONE
};

typedef ModuleType_t (*getTypeFunc_t)();

#endif /* !LOADERTYPE_HPP_ */
