#ifndef FPP_ERROR_HPP
#define FPP_ERROR_HPP

#include <string>
#include <concepts> // for: std::convertible_to<>;

#include "../Result/Result.hpp"
#include "ErrorConcept.hpp"

namespace fpp {

//* <--- Basic wrappers for classic error handling methods --->
struct StringError {
    std::string msg = "Undefined error";
    std::string err_message() const noexcept { return msg; }

    bool operator==(const StringError& oth) const noexcept{
        return msg == oth.msg;
    }
};

struct CodeError {
    int code;
    std::string err_message() const noexcept { return "Error code: " + std::to_string(code); }
};

// TODO :
struct ExceptionError {
    // . . .
};

//* <--- Functions that can help to process code written in the style of exception handling, in the style of error handling --->

//? Forward-declaration:
// template<typename T, typename E> requires Error<E>
// class Result;

// template<typename Fn, typename... Args>
// auto capture_exception(Fn&& fn, Args&&... args) -> Result<std::invoke_result_t<Fn, Args...>, ExceptionError>;

//? analog of 'try', which converts the received exception to Error.
// template<typename Fn, typename... Args>
// auto try_or_convert(Fn&& fn, Args&&... args) -> Result<std::invoke_result_t<Fn, Args...>, ExceptionError>;

} // namespace 'fpp'

// #include "../../src/Error/Error.tpp"

#endif // FPP_ERROR_HPP