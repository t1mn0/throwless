#ifndef THROWLESS_OPTION_HPP
#error "Include Option.hpp instead of Option.tpp"
#endif

#include <utility> // for: move, swap;
#include <optional> // for: bad_optional_access (exception);
#include <type_traits> // for: is_void_v;
#include <functional> // for: function;

#include "Option.hpp" // for: Option declaration;

namespace tmn::err {

//*   <--- constructors, (~)ro5, destructor --->

template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
Option<T>::Option() noexcept : _is_initialized(false) {}

template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
Option<T>::Option(const T& val) noexcept(std::is_nothrow_copy_constructible_v<T>) : _is_initialized(true) {
  new (_value) T(val);
}

template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
Option<T>::Option(T&& val) noexcept(std::is_nothrow_move_constructible_v<T>) : _is_initialized(true) {
  new (_value) T(std::move(val));
}

template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
Option<T>::Option(const Option<T>& oth) noexcept(std::is_nothrow_copy_constructible_v<T>) : _is_initialized(oth._is_initialized) {
  if (_is_initialized) {
    new (_value) T(*reinterpret_cast<const T*>(oth._value));
  }
}

template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
Option<T>::Option(Option<T>&& oth) noexcept(std::is_nothrow_move_constructible_v<T>) : _is_initialized(oth._is_initialized) {
  if (_is_initialized) {
    new (_value) T(std::move(*reinterpret_cast<T*>(oth._value)));
    reinterpret_cast<T*>(oth._value)->~T();
    oth._is_initialized = false;
  }
}

template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
Option<T>& Option<T>::operator=(const Option<T>& oth) noexcept(std::is_nothrow_copy_constructible_v<T> && std::is_nothrow_copy_assignable_v<T>) {
  Option<T> tmp(oth);
  swap(tmp);
  return *this;
}

template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
Option<T>& Option<T>::operator=(Option<T>&& oth) noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_assignable_v<T>) {
  _is_initialized = oth._is_initialized;
  if (_is_initialized) {
    new (_value) T(std::move(*reinterpret_cast<T*>(oth._value)));
    reinterpret_cast<T*>(oth._value)->~T();
    oth._is_initialized = false;
  }
  return *this;
}

template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
void Option<T>::swap(Option<T>& oth) noexcept(std::is_nothrow_swappable_v<T> && std::is_nothrow_move_constructible_v<T>) {
  std::swap(_value, oth._value);
  std::swap(_is_initialized, oth._is_initialized);
}

template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
Option<T>::operator bool() const noexcept {
  return _is_initialized;
}

template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
Option<T>::~Option(){
  destroy_value();
}

//*   <--- specialized algorithms & methods  --->

template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
bool Option<T>::has_value() const noexcept {
  return _is_initialized;
}

template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
T& Option<T>::value_or(T& val) noexcept {
  return _is_initialized ? *reinterpret_cast<T*>(_value) : val;
}

template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
const T& Option<T>::value_or(const T& val) const noexcept {
  return _is_initialized ? *reinterpret_cast<const T*>(_value) : val;
}

template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
T Option<T>::value_or_default() noexcept requires std::default_initializable<T> {
  return _is_initialized ? *reinterpret_cast<const T*>(_value) : T{};
}

template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
const T& Option<T>::value() const {
  if (!_is_initialized) throw std::bad_optional_access();
  return *reinterpret_cast<const T*>(_value);
}

template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
T& Option<T>::value() {
  if (!_is_initialized) throw std::bad_optional_access();
  return *reinterpret_cast<T*>(_value);
}

template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
bool Option<T>::destroy_value() noexcept {
  if (_is_initialized) {
    reinterpret_cast<T*>(_value)->~T();
    _is_initialized = false;
    return true;
  }
  return false;
}

template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
bool Option<T>::operator==(const Option& oth) const noexcept {
  if (_is_initialized != oth._is_initialized) return false;
  if (!_is_initialized) return true;
  return *reinterpret_cast<T*>(_value) == *reinterpret_cast<T*>(oth._value);
}

//  <--- cast to other classes --->

template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
template <typename E> requires (!std::is_void_v<T> && Error<E>)
Result<T, E> Option<T>::to_result(E error_if_none) {
  if (has_value()) {
    return Result<T, E>::Ok(value());
  }
  return Result<T, E>::Err(error_if_none);
}

//*   <--- functional methods indicating relationship to the type of func_structure  --->

// Functor interface:
template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
template <typename Func> requires std::invocable<Func, T>
auto Option<T>::fmap(Func&& fn)
  noexcept(std::is_nothrow_constructible_v<Option<std::invoke_result_t<Func, T>>> && std::is_nothrow_invocable_v<Func, T>)
  -> Option<std::invoke_result_t<Func, T>>
{
  if (!_is_initialized) return Option<std::invoke_result_t<Func, T>>{};
  return Option<std::invoke_result_t<Func, T>>(fn(*reinterpret_cast<T*>(_value)));
}

// Applicative interface:
template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
template<typename Func>
requires std::invocable<Func, T>
auto Option<T>::apply(const Option<Func>& fn_option) const
  noexcept(std::is_nothrow_constructible_v<Option<std::invoke_result_t<Func, T>>> && std::is_nothrow_invocable_v<Func, T>)
  -> Option<std::invoke_result_t<Func, T>>
{
  if (_is_initialized && fn_option._is_initialized) {
    return Option<std::invoke_result_t<Func, T>>(fn_option.value()(_value));
  }
  else {
    return Option<std::invoke_result_t<Func, T>>{};
  }
}

// Monad interface:
template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
template <typename Func> requires std::invocable<Func, T>
auto Option<T>::and_then(Func&& fn) const
  noexcept(std::is_nothrow_constructible_v<Option<std::invoke_result_t<Func, T>>> && std::is_nothrow_invocable_v<Func, T>)
  -> Option<std::invoke_result_t<Func, T>>
{
  if (!_is_initialized) return std::invoke_result_t<Func, T>{};
  return Option<std::invoke_result_t<Func, T>>(std::forward<Func>(fn)(*reinterpret_cast<const T*>(_value)));
}

// For convenient chaining:
template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
template <typename Func, typename... Args> requires std::invocable<Func, Args...>
auto Option<T>::or_else(Func&& fn, Args&&... args) const
  noexcept(std::is_nothrow_constructible_v<Option<std::invoke_result_t<Func, T>>> && std::is_nothrow_invocable_v<Func, T>)
  -> Option<std::invoke_result_t<Func, T>>
{
  if (_is_initialized) return std::invoke_result_t<Func, T>{};
  return Option<std::invoke_result_t<Func, T>>(std::forward<Func>(fn)(std::forward<Args>(args)...));
}

// Monoid interface:
template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
template <typename U> requires Addable<T, U>
Option<T>& Option<T>::operator+=(const Option<U>& rhs) {
  if (has_value() && rhs.has_value()) {
    *reinterpret_cast<T*>(_value) += rhs.value();
  }
  else {
    _is_initialized = false;
  }
  return *this;
}

template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
template <typename U> requires Subtractable<T, U>
Option<T>& Option<T>::operator-=(const Option<U>& rhs) {
  if (has_value() && rhs.has_value()) {
    *reinterpret_cast<T*>(_value) -= rhs.value();
  }
  else {
    _is_initialized = false;
  }
  return *this;
}

template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
template <typename U> requires Multipliable<T, U>
Option<T>& Option<T>::operator*=(const Option<U>& rhs) {
  if (has_value() && rhs.has_value()) {
    *reinterpret_cast<T*>(_value) *= rhs.value();
  }
  else {
    _is_initialized = false;
  }
  return *this;
}

template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
template <typename U> requires Dividable<T, U>
Option<T>& Option<T>::operator/=(const Option<U>& rhs) {
  if (has_value() && rhs.has_value()) {
    *reinterpret_cast<T*>(_value) /= rhs.value();
  }
  else {
    _is_initialized = false;
  }
  return *this;
}

//* <--- external mnemonic functions --->

template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
Option<T> None() noexcept {
  return Option<T>();
}

template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
Option<T> Some(T val) noexcept(std::is_nothrow_move_constructible_v<T>) {
  return Option<T>(std::move(val));
}

static_assert(Functor<Option, int, std::function<double(int)>>, "Option should be a Functor");
static_assert(Applicative<Option, int, std::function<double(int)>>, "Option should be an Applicative");
static_assert(Monad<Option, int, std::function<Option<double>(int)>>, "Option should be a Monad");

} // namespace 'tmn::err';

// Specialization of std::swap delegating to the Option<T> private method:
namespace std {

template <typename T> requires (!std::is_void_v<T>)
void swap(tmn::err::Option<T>& a, tmn::err::Option<T>& b) noexcept(noexcept(a.swap(b))) {
  a.swap(b);
}

} // namespace 'std';
