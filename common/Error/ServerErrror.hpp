/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ServerErrror
*/


#include "AError.hpp"

namespace err {

class ServerError : public AError {
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

        ServerError(const std::string &message, ErrorCode code = UNKNOWN);
        virtual ~ServerError() noexcept = default;
        std::string getType() const noexcept override;

    private:
};

}
