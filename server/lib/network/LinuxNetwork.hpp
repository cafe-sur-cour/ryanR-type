/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LinuxNetwork
*/

#include "ANetwork.hpp"

#ifndef LINUXNETWORK_HPP_
#define LINUXNETWORK_HPP_

class LinuxNetwork : public ANetwork {
    public:
        LinuxNetwork();
        ~LinuxNetwork() override;

        void init() override;
        void stop() override;
        void sendData(const IPacket &data, size_t size) override;
        IPacket &receiveData(char *buffer, size_t size) const override;
    protected:
    private:
};

extern "C" {
    INetwork *createNetworkInstance();
    int getType();
}
#endif /* !LINUXNETWORK_HPP_ */
