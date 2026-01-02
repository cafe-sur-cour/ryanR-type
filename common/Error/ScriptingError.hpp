/*
** EPITECH PROJECT, 2026
** ryanR-type
** File description:
** ScriptingError
*/

#ifndef SCRIPTINGERROR_HPP_
#define SCRIPTINGERROR_HPP_


#include "AError.hpp"

namespace err {

class ScriptingError : public AError {
    public:
        enum ErrorCode {
            UNKNOWN = 1000,
            LOAD_FAILED = 1001,
            RUN_FAILED = 1002
        };

        ScriptingError(const std::string &message, ErrorCode code = UNKNOWN);
        ~ScriptingError() noexcept = default;
        std::string getType() const noexcept override;

    protected:
    private:
};

}  // namespace err

#endif /* !SCRIPTINGERROR_HPP_ */
