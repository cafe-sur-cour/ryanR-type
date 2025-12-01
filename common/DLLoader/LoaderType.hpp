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
#define multimediaLib "libMultimedia"
#define networkLib "libNetwork"
#define bufferLib "libBuffer"
#define packetLib "libPacket"


#ifdef _WIN32
    #define sharedLibExt ".dll"
#elif __APPLE__
    #define sharedLibExt ".dylib"
#else
    #define sharedLibExt ".so"

#endif

#endif /* !LOADERTYPE_HPP_ */
