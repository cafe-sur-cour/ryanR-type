/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LibrairiesLoadError
*/

#ifndef LIBRAIRIESLOADERROR_HPP_
#define LIBRAIRIESLOADERROR_HPP_

#include "AError.hpp"

namespace err {

class LibrairiesLoadError : public AError  {
    public:
        enum ErrorCode {
            UNKNOWN = 1000,
            LIBRARY_NOT_FOUND = 1001,
            SYMBOL_NOT_FOUND = 1002
        };

        LibrairiesLoadError(const std::string &message, ErrorCode code = UNKNOWN);
        ~LibrairiesLoadError() override;
        std::string getType() const noexcept override;

    protected:
    private:
};

}

#endif /* !LIBRAIRIESLOADERROR_HPP_ */
