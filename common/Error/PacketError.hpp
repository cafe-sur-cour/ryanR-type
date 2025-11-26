/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Header
*/

#ifndef PACKET_ERROR_HPP
    #define PACKET_ERROR_HPP

#include "AError.hpp"

namespace err {

class PacketError : public AError {
    public:
        enum ErrorCode {
            UNKNOWN = 1000,
            LIBRARY_LOAD_FAILED = 1001
        };

        PacketError(const std::string &message, ErrorCode code = UNKNOWN);
        ~PacketError() override;
        std::string getType() const noexcept override;

    protected:
    private:
};

}

#endif /* !PACKET_ERROR_HPP */
