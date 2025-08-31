#ifndef THROWLESS_RESULT_HPP
#error "Include Result.hpp instead of Result.tpp"
#endif

#include <stdexcept> // for: runtime_error;
#include <utility> // for: move;

#include "Result.hpp"
#include "../Error/ErrorConcept.hpp"

namespace tmn::err {

//*   <--- constructors, (~)ro5, destructor --->

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
Result<T, E>::Result(const T& val) noexcept(std::is_nothrow_copy_constructible_v<T>)
  : state(State::OkState), ok_val(val) {}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
Result<T, E>::Result(T&& val) noexcept(std::is_nothrow_move_constructible_v<T>)
  : state(State::OkState), ok_val(std::move(val)) {}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
Result<T, E>::Result(const E& err) noexcept(std::is_nothrow_copy_constructible_v<E>)
  : state(State::ErrState), err_val(err) {}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
Result<T, E>::Result(E&& err) noexcept(std::is_nothrow_move_constructible_v<E>)
  : state(State::ErrState), err_val(std::move(err)) {}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
Result<T,E> Result<T, E>::Ok(const T& val) noexcept requires (!std::is_void_v<T>) {
  return Result(val);
}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
Result<T,E> Result<T, E>::Ok(T&& val) noexcept requires (!std::is_void_v<T>) {
  return Result(val);
}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
Result<T,E> Result<T, E>::Err(const E& err) noexcept {
  return Result(err);
}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
Result<T,E> Result<T, E>::Err(E&& err) noexcept {
  return Result(std::move(err));
}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
Result<T, E>::Result(const Result& oth)
  noexcept(std::is_nothrow_copy_constructible_v<T> && std::is_nothrow_copy_constructible_v<E>)
{
  if (oth.state == State::OkState) {
    new (&ok_val) T(oth.ok_val);
    state = State::OkState;
  }
  else if (oth.state == State::ErrState) {
    new (&err_val) E(oth.err_val);
    state = State::ErrState;
  }
  else if (oth.state == State::Uninitialized) {
    state = State::Uninitialized;
  }
}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
Result<T, E>::Result(Result&& oth)
  noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_constructible_v<E>)
{
  if (oth.is_ok()) {
    new (&ok_val) T(std::move(oth.ok_val));
    state = State::OkState;
    oth.ok_val.~T();
    oth.state = State::Uninitialized;
  }
  else {
    new (&err_val) E(std::move(oth.err_val));
    state = State::ErrState;
    oth.err_val.~E();
    oth.state = State::Uninitialized;
  }
}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
Result<T,E>& Result<T, E>::operator=(const Result<T,E>& oth)
  noexcept(std::is_nothrow_copy_constructible_v<T> && std::is_nothrow_copy_constructible_v<E>)
{
  if (this == &oth) {
    return *this;
  }

  if (state == State::OkState) {
    ok_val.~T();
  }
  else if (state == State::ErrState) {
    err_val.~E();
  }

  state = oth.state;
  if (state == State::OkState) {
    new (&ok_val) T(oth.ok_val);
  }
  else if (state == State::ErrState) {
    new (&err_val) E(oth.err_val);
  }

  return *this;
}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
Result<T,E>& Result<T,E>::operator=(Result<T,E>&& oth)
    noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_constructible_v<E>)
{
  if (this == &oth) {
    return *this;
  }

  if (state == State::OkState) {
    ok_val.~T();
  }
  else if (state == State::ErrState) {
    err_val.~E();
  }

  state = oth.state;
  if (state == State::OkState) {
    new (&ok_val) T(std::move(oth.ok_val));
    oth.ok_val.~T();
  }
  else if (state == State::ErrState) {
    new (&err_val) E(std::move(oth.err_val));
    oth.err_val.~E();
  }

  return *this;
}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
Result<T, E>::~Result() {
  if (state == State::OkState) {
    ok_val.~T();
  }
  else if (state == State::ErrState) {
    err_val.~E();
  }
}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
void Result<T, E>::swap(Result<T, E> &oth)
    noexcept(std::is_nothrow_swappable_v<T> && std::is_nothrow_move_constructible_v<T> &&
             std::is_nothrow_swappable_v<E> && std::is_nothrow_move_constructible_v<E>)
{
  if (this == &oth) return;

  Result<T,E> temp(std::move(*this));
  *this = std::move(oth);
  oth = std::move(temp);
}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
Result<T, E>::operator bool() const noexcept{
  return is_ok();
}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
Option<T> Result<T, E>::to_option() {
  if (is_ok()) {
    return Option<T>(ok_val);
  }
  return Option<T>();
}

//*   <--- specialized algorithms & methods  --->

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
bool Result<T,E>::is_ok() const noexcept {
  return state == State::OkState;
}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
bool Result<T,E>::is_err() const noexcept {
  return state == State::ErrState;
}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
Option<T> Result<T, E>::optional_value() const noexcept(std::is_nothrow_copy_constructible_v<T>) {
  if (is_ok()) return Option<T>(ok_val);
  return Option<T>();
}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
T& Result<T,E>::value_or(T& val) noexcept(std::is_nothrow_copy_constructible_v<T>) {
  return is_ok() ? ok_val : val;
}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
const T& Result<T,E>::value_or(const T& val) const
  noexcept(std::is_nothrow_copy_constructible_v<T>)
{
  return is_ok() ? ok_val : val;
}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
T& Result<T, E>::value() {
  if (is_err()) {
    throw std::runtime_error(err_val.err_message());
  }
  else if (state == State::Uninitialized) {
    throw std::runtime_error("Accessing moved-from Result");
  }

  return ok_val;
}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
const T& Result<T, E>::value() const {
  if (is_err()) {
    throw std::runtime_error(err_val.err_message());
  }
  return ok_val;
}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
T Result<T, E>::value_or_default() const noexcept requires std::default_initializable<T> {
  return is_ok() ? ok_val : T{};
}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
Option<E> Result<T, E>::optional_err() const noexcept(std::is_nothrow_copy_constructible_v<E>) {
  if (is_err()) return Option<E>(err_val);
  return Option<E>();
}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
E& Result<T, E>::err() {
  if (is_ok()) {
    throw std::runtime_error("Result<T, E> does not contain Error");
  }
  return err_val;
}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
const E& Result<T, E>::err() const {
  if (is_ok()) {
    throw std::runtime_error("Result<T, E> does not contain Error");
  }
  return err_val;
}

//*   <--- functional methods  --->

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
template <typename Func> requires std::invocable<Func, T>
auto Result<T, E>::fmap(Func&& fn)
  noexcept(std::is_nothrow_constructible_v<Result<std::invoke_result_t<Func, T>, E>> && std::is_nothrow_invocable_v<Func, T>)
  -> Result<std::invoke_result_t<Func, T>, E>
{
  if (is_ok()) {
    return Result<std::invoke_result_t<Func, T>, E>(std::forward<Func>(fn)(ok_val));
  }
  else {
    return Result<std::invoke_result_t<Func, T>, E>(err_val);
  }
}

template<typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
template<typename Func> requires std::invocable<Func, E>
auto Result<T, E>::fmap_err(Func&& fn) const
  noexcept(std::is_nothrow_constructible_v<Result<T, std::invoke_result_t<Func, E>>> && std::is_nothrow_invocable_v<Func, T>)
  -> Result<T, std::invoke_result_t<Func, E>>
{
  if (is_err()) {
    return Result<T, std::invoke_result_t<Func, E>>(std::forward<Func>(fn)(err_val));
  }
  else {
    return Result<T, std::invoke_result_t<Func, E>>(ok_val);
  }
}

template<typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
template <typename Func>
requires std::invocable<Func, T> && std::same_as<std::invoke_result_t<Func, T>, Result<std::invoke_result_t<Func, T>, E>>
auto Result<T, E>::and_then(Func&& fn) const
  noexcept(std::is_nothrow_constructible_v<Result<std::invoke_result_t<Func, T>, E>> && std::is_nothrow_invocable_v<Func, T>)
  -> std::invoke_result_t<Func, T>
{
  if (is_ok()) {
    return fn(ok_val);
  }
  else {
    return std::invoke_result_t<Func, T>(err_val);
  }
}

} // namespace 'tmn::err';

namespace std {

template <typename T, typename E> requires (!std::is_void_v<T> && tmn::err::Error<E>)
void swap(tmn::err::Result<T,E>& a, tmn::err::Result<T,E>& b) noexcept(noexcept(a.swap(b))) {
  a.swap(b);
}

} // namespace 'std';
