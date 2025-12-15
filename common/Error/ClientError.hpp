/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ClientError
*/

#ifndef CLIENTERROR_HPP_
#define CLIENTERROR_HPP_

#include "AError.hpp"

namespace err {

class ClientError : public AError {
    public:
        enum ErrorCode {
            UNKNOWN = 2000,
            CONNECTION_FAILED = 2001,
            DISCONNECTED = 2002,
            TIMEOUT = 2003,
            NOT_INITIALIZED = 2004,
            CAN_NOT_OPEN_FILE = 2005
        };

        ClientError(const std::string &message, ErrorCode code = UNKNOWN);
        ~ClientError() override;

        std::string getType() const noexcept override;
    protected:
    private:
};

}  // namespace err

#endif /* !CLIENTERROR_HPP_ */
