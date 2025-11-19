/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** LoaderType
*/

#ifndef LOADERTYPE_HPP_
#define LOADERTYPE_HPP_

enum ModuleType_t{
    DISPLAY_MODULE, /* 0 */
    NETWORK_MODULE, /* 1 */
    PACKET_MODULE, /* 2 */
    BUFFER_MODULE, /* 3 */
    UNKNOWN_MODULE /* 4 */
};

typedef ModuleType_t (*getTypeFunc_t)();

#endif /* !LOADERTYPE_HPP_ */
