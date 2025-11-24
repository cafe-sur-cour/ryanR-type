/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** UnixNetwork
*/

#ifndef UnixNetwork_HPP_
#define UnixNetwork_HPP_

#include "../ANetwork.hpp"
#include "../../Buffer/IBuffer.hpp"
#include "../../Packet/IPacket.hpp"

namespace net {

class UnixNetwork : public ANetwork {
    public:
        UnixNetwork();
        ~UnixNetwork() override;

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

#endif /* !UnixNetwork_HPP_ */
