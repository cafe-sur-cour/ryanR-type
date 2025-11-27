/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** LoaderType
*/

#ifndef LOADERTYPE_HPP_
#define LOADERTYPE_HPP_

enum ModuleType_t{
    MULTIMEDIA_MODULE = 0,
    NETWORK_MODULE = 1,
    PACKET_MODULE = 2,
    BUFFER_MODULE = 3,
    UNKNOWN_MODULE
};

typedef ModuleType_t (*getTypeFunc_t)();

typedef void *(*createNetworkLib_t)();
typedef void *(*createBuffer_t)();
typedef void *(*createPacket_t)();

#define pathLoad "./libraries"

#ifdef _WIN32
    #define multimediaLib "libMultimedia.dll"
    #define networkLib "libNetwork.dll"
    #define bufferLib "libBuffer.dll"
    #define packetLib "libPacket.dll"
#else
    #define multimediaLib "libMultimedia.so"
    #define networkLib "libNetwork.so"
    #define bufferLib "libBuffer.so"
    #define packetLib "libPacket.so"
#endif

#endif /* !LOADERTYPE_HPP_ */
