/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** WindowsNetwork
*/
#include "ANetwork.hpp"

#ifndef WINDOWSNETWORK_HPP_
#define WINDOWSNETWORK_HPP_

class WindowsNetwork : public ANetwork {
    public:
        WindowsNetwork();
        ~WindowsNetwork() override;

        void init() override;
        void stop() override;
        void sendData(const IPacket &data, size_t size) override;
        IPacket &receiveData(const IBuffer &buffer, size_t size) const override;
    protected:
    private:
};

extern "C" {
    INetwork *createNetworkInstance();
    int getType();
}

#endif /* !WINDOWSNETWORK_HPP_ */
