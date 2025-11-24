/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LinuxNetwork
*/

#ifndef LINUXNETWORK_HPP_
#define LINUXNETWORK_HPP_

#include "../ANetwork.hpp"
#include "../../Buffer/IBuffer.hpp"
#include "../../Packet/IPacket.hpp"

namespace net {

class LinuxNetwork : public ANetwork {
    public:
        LinuxNetwork();
        ~LinuxNetwork() override;

        void init() override;
        void stop() override;
        void sendData(const IPacket &data, size_t size) override;
        IPacket &receiveData(const IBuffer &buffer, size_t size) const override;
    protected:
    private:
};

} // namespace net

extern "C" {
    void *createNetworkInstance();
    int getType();
}

#endif /* !LINUXNETWORK_HPP_ */
