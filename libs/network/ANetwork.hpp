/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ANetwork
*/
#include "INetwork.hpp"

#ifndef ANETWORK_HPP_
#define ANETWORK_HPP_

namespace net {

class ANetwork : public INetwork {
    public:

        ~ANetwork() override;

        virtual void init() override = 0;
        virtual void stop() override = 0;
        virtual void sendData(const IPacket &data, size_t size) override = 0;
        virtual IPacket &receiveData(const IBuffer &buffer, size_t size) const override = 0;
    protected:
    private:
};

} // namespace net

#endif /* !ANETWORK_HPP_ */
