#ifndef THROWLESS_TRY_OR_CONVERT_HPP
#define THROWLESS_TRY_OR_CONVERT_HPP

//* <--- Functions that can help to process code written in the style of exception handling, in the style of error handling --->

#include <type_traits> // for: invoke_result_t;
#include <functional> // for: invoke;

#include "Error.hpp" // for: ExceptionErr;
#include "ErrorConcept.hpp" // for: Error<E>;

namespace tmn::err {

// forward-declaration:
template<typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
class Result;

// analog of 'try', which converts the received exception to Error:
template<typename Fn, typename... Args>
auto try_or_convert(Fn&& fn, Args&&... args) -> Result<std::invoke_result_t<Fn, Args...>, ExceptionErr> {
  try {
    return Result<std::invoke_result_t<Fn, Args...>, ExceptionErr>::Ok(std::invoke(std::forward<Fn>(fn), std::forward<Args>(args)...));
  } catch (const std::exception& e) {
    return Result<std::invoke_result_t<Fn, Args...>, ExceptionErr>::Err(ExceptionErr{e});
  } catch (...) {
    return Result<std::invoke_result_t<Fn, Args...>, ExceptionErr>::Err(ExceptionErr{});
  }
}

};

#endif // THROWLESS_TRY_OR_CONVERT_HPP
