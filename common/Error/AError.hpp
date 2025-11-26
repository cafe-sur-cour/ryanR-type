/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AError
*/

#ifndef AERROR_HPP_
#define AERROR_HPP_

#include <string>
#include "IError.hpp"

namespace err {

class AError : public IError {
    public:
        AError(const std::string &message, int code = 0);

        virtual ~AError() noexcept = default;
        const char *what() const noexcept override;
        int getCode() const noexcept;
        std::string getDetails() const noexcept;

        virtual std::string getType() const noexcept = 0;

    protected:
        std::string m_message;
        int m_code;
};

}

#endif /* !AERROR_HPP_ */
