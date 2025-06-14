#ifndef ERROR_H
#define ERROR_H

#include <string>
#include <concepts> // for: std::convertible_to<>;
#include "../Result/Result.hpp"

namespace throwless {

//* <--- The basic concept that the "Error" type structure should correspond to --->
template<typename E>
concept Error = requires(E e) {
    { e.message() } -> std::convertible_to<std::string>;
};

//* <--- Basic wrappers for classic error handling methods --->
struct StringError {
    std::string msg;
    std::string message() const noexcept { return msg; }
};

struct CodeError {
    int code;
    std::string message() const noexcept { return "Error code: " + std::to_string(code); }
};

struct ExceptionError {
    std::exception_ptr ptr;
    std::string context;

public:
    explicit ExceptionError(std::exception_ptr ptr, std::string context = "") noexcept;
    explicit ExceptionError(std::string context = "") noexcept;
    
    std::string message() const noexcept;
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
auto capture_exception(Fn&& fn, Args&&... args) -> Result<std::invoke_result_t<Fn>, ExceptionError>;

template<typename Fn, typename... Args>
auto try_or_convert(Fn&& fn, Args&&... args) -> Result<std::invoke_result_t<Fn>, ExceptionError>;

} // end of namespace 'throwless'

#endif