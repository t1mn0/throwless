#ifndef FPP_ERROR_HPP
#error "Include Error.hpp instead of Error.tpp"
#endif

#include "../../include/Error/Error.hpp"

#include <utility> // for: std::forward, std::move;
#include <exception> // for: std::current_exception;

namespace fpp {

//*   <--- struct ExceptionError --->
// TODO : ExceptionError struct

//* <--- Functions that can help to process code written in the style of exception handling, in the style of error handling --->

// template<typename Fn, typename... Args>
// auto capture_exception(Fn&& fn, Args&&... args) -> Result<std::invoke_result_t<Fn, Args...>, ExceptionError> {
//     try {
//         if constexpr (std::is_void_v<std::invoke_result_t<Fn, Args...>>) {
//             std::forward<Fn>(fn)(std::forward<Args>(args)...);
//             return {};
//         } else {
//             return Result<std::invoke_result_t<Fn, Args...>, ExceptionError>::Ok(std::forward<Fn>(fn)(std::forward<Args>(args)...));
//         }
//     } catch (...) {
//         return Result<std::invoke_result_t<Fn, Args...>, ExceptionError>::Err(ExceptionError(std::current_exception()));
//     }
// }

// template <typename Fn, typename... Args>
// auto fpp::try_or_convert(Fn&& fn, Args&&...args) -> Result<std::invoke_result_t<Fn, Args...>, ExceptionError> {
//     return capture_exception(std::forward<Fn>(fn), std::forward<Args>(args)...);
// }
    
} // namespace 'fpp'