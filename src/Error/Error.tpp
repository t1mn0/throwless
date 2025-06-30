#include "../../include/Error/Error.hpp"

#ifndef ERROR_H
#error "Include Error.hpp instead of Error.tpp"
#endif

#include <utility> // for: std::forward, std::move;
#include <exception> // for: std::current_exception;

namespace throwless {

//*   <--- struct ExceptionError --->

ExceptionError::ExceptionError(std::exception_ptr ptr, std::string context) noexcept 
    : ptr(std::move(ptr)), context(std::move(context)) {}
    
ExceptionError::ExceptionError(std::exception_ptr ptr) noexcept 
    : ptr(std::move(ptr)) {}

ExceptionError::ExceptionError(std::string context) noexcept 
    : ptr(std::current_exception()), context(std::move(context)) {}

ExceptionError::ExceptionError() noexcept 
    : ptr(std::current_exception()) {}

ExceptionError::ExceptionError(ExceptionError&& other) noexcept
    : ptr(std::move(other.ptr)), context(std::move(other.context)) {
    other.ptr = nullptr;
}

ExceptionError& ExceptionError::operator=(ExceptionError&& other) noexcept {
    if (this != &other) {
        ptr = std::move(other.ptr);
        context = std::move(other.context);
        other.ptr = nullptr;
    }
    return *this;
}

ExceptionError::~ExceptionError() {
    if (ptr) {
        try {
            std::rethrow_exception(ptr);
        } 
        catch (...) {}
    }
}

//* <--- Functions that can help to process code written in the style of exception handling, in the style of error handling --->

template<typename Fn, typename... Args>
auto capture_exception(Fn&& fn, Args&&... args) -> Result<std::invoke_result_t<Fn, Args...>, ExceptionError> {
    try {
        if constexpr (std::is_void_v<std::invoke_result_t<Fn, Args...>>) {
            std::forward<Fn>(fn)(std::forward<Args>(args)...);
            return {};
        } else {
            return Result<std::invoke_result_t<Fn, Args...>, ExceptionError>::Ok(std::forward<Fn>(fn)(std::forward<Args>(args)...));
        }
    } catch (...) {
        return Result<std::invoke_result_t<Fn, Args...>, ExceptionError>::Err(ExceptionError(std::current_exception()));
    }
}

template <typename Fn, typename... Args>
auto throwless::try_or_convert(Fn&& fn, Args&&...args) -> Result<std::invoke_result_t<Fn, Args...>, ExceptionError> {
    return capture_exception(std::forward<Fn>(fn), std::forward<Args>(args)...);
}
    
} // end of namespace 'throwless'