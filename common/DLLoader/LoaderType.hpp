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
    NETWORK_SERVER_MODULE = 1,
    NETWORK_CLIENT_MODULE = 2,
    PACKET_MODULE = 3,
    BUFFER_MODULE = 4,
    UNKNOWN_MODULE
};

typedef ModuleType_t (*getTypeFunc_t)();

typedef void *(*createNetworkLib_t)();
typedef void *(*createBuffer_t)();
typedef void *(*createPacket_t)();

#define pathLoad "./libraries"

#ifdef _WIN32
    #define multimediaLib "Multimedia"
    #define networkServerLib "NetworkServer"
    #define networkClientLib "NetworkClient"
    #define bufferLib "Buffer"
    #define packetLib "Packet"
    #define sharedLibExt ".dll"
#elif __APPLE__
    #define multimediaLib "libMultimedia"
    #define networkServerLib "libNetworkServer"
    #define networkClientLib "libNetworkClient"
    #define bufferLib "libBuffer"
    #define packetLib "libPacket"
    #define sharedLibExt ".dylib"
#else
    #define multimediaLib "libMultimedia"
    #define networkServerLib "libNetworkServer"
    #define networkClientLib "libNetworkClient"
    #define bufferLib "libBuffer"
    #define packetLib "libPacket"
    #define sharedLibExt ".so"
#endif

#endif /* !LOADERTYPE_HPP_ */
