#ifndef ERROR_H
#define ERROR_H

#include <string>
#include <concepts> // for: std::convertible_to<>;

#include "../Result/Result.hpp"
#include "ErrorConcept.hpp"

namespace throwless {

//* <--- Basic wrappers for classic error handling methods --->
struct StringError {
    std::string msg = "Undefined error";
    std::string message() const noexcept { return msg; }
};

struct CodeError {
    int code;
    std::string message() const noexcept { return "Error code: " + std::to_string(code); }
};

struct ExceptionError {
    std::exception_ptr ptr;
    std::string context = "empty Context";

public:
    explicit ExceptionError(std::exception_ptr ptr, std::string context) noexcept;
    explicit ExceptionError(std::exception_ptr ptr) noexcept;
    explicit ExceptionError(std::string context) noexcept;
    explicit ExceptionError() noexcept;
    ExceptionError(const ExceptionError&) = delete;
    ExceptionError& operator=(const ExceptionError&) = delete;
    ExceptionError(ExceptionError&& other) noexcept;
    ExceptionError& operator=(ExceptionError&& other) noexcept;
    ~ExceptionError();
    
    
    std::string message() const noexcept { return "ExceptionError with context: '" + context + "'"; }
};

//* <--- Functions that can help to process code written in the style of exception handling, in the style of error handling --->

// Forward-declaration: 
//  we include Error.hpp before the Result.hpp, 
//  since the Result class depends on the Error 
//  concept, which is located in the Error.hpp file.
template<typename T, typename E> requires Error<E>
class Result;

// we use the universal reference `Fn&&` (saves the value category (lvalue/rvalue) of the passed object)
template<typename Fn, typename... Args>
auto capture_exception(Fn&& fn, Args&&... args) -> Result<std::invoke_result_t<Fn, Args...>, ExceptionError>;

template<typename Fn, typename... Args>
auto try_or_convert(Fn&& fn, Args&&... args) -> Result<std::invoke_result_t<Fn, Args...>, ExceptionError>;

} // end of namespace 'throwless'

#include "../../src/Error/Error.tpp"

#endif