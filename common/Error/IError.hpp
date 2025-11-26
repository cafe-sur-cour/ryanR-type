/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** IError
*/

#ifndef IERROR_HPP_
#define IERROR_HPP_

#include <string>
#include <exception>

namespace err {

class IError : public std::exception {
    public:

        virtual ~IError() noexcept = default;
        virtual const char *what() const noexcept override = 0;
        virtual int getCode() const noexcept = 0;
        virtual std::string getType() const noexcept = 0;
        virtual std::string getDetails() const noexcept = 0;

    protected:
    private:
};

}

#endif /* !IERROR_HPP_ */

