/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ClientNetworkError
*/

#ifndef CLIENTNETWORKERROR_HPP_
#define CLIENTNETWORKERROR_HPP_

#include "AError.hpp"

namespace err {

class ClientNetworkError : public AError {
    public:
        enum ErrorCode {
            UNKNOWN = 1000,
            CONNECTION_FAILED = 1001,
            TIMEOUT = 1002,
            INVALID_REQUEST = 1003,
            INTERNAL_ERROR = 1004,
            LIBRARY_LOAD_FAILED = 1005,
            CONFIG_ERROR = 1006
        };

        ClientNetworkError(const std::string &message, ErrorCode code = UNKNOWN);
        virtual ~ClientNetworkError() noexcept = default;
        std::string getType() const noexcept override;

    private:
};

}

#endif /* !CLIENTNETWORKERROR_HPP_ */
