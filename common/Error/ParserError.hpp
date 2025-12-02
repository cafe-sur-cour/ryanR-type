/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ParserError
*/

#ifndef PARSERERROR_HPP_
#define PARSERERROR_HPP_

#include "AError.hpp"
namespace err {

class ParserError : public AError {
    public:
        enum ErrorCode {
            UNKNOWN = 1000,
            FILE_NOT_FOUND = 1001,
            INVALID_FORMAT = 1002,
            MISSING_FIELD = 1003,
            TYPE_MISMATCH = 1004
        };

        ParserError(const std::string &message, ErrorCode code = UNKNOWN);
        virtual ~ParserError() noexcept = default;
        std::string getType() const noexcept override;
};

}  // namespace err

#endif /* !PARSERERROR_HPP_ */
