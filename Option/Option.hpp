#ifndef THROWLESS_OPTION_HPP
#define THROWLESS_OPTION_HPP

#include <concepts> // for: default_initializable, invocable;
#include <type_traits> // for is_nothrow_constructible;

#include "../Concepts/Concepts.hpp"
#include "../Error/ErrorConcept.hpp"

namespace tmn::err {

// forward-declaration:
template<typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
class Result;

template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
class Option {
private: //* fields :
  // always allocate sizeof(T) bytes for the value;
  // if there is no value (_is_initialized = false),
  // garbage is stored in the array, and an object
  // of type T is not built.
  alignas(T) char _value[sizeof(T)];
  bool      _is_initialized = false;

public: //* methods :
  //*   <--- constructors, (~)ro5, destructor --->
  Option() noexcept;
  explicit Option(const T& val) noexcept(std::is_nothrow_copy_constructible_v<T>);
  explicit Option(T&& val) noexcept(std::is_nothrow_move_constructible_v<T>);

  Option(const Option& oth) noexcept(std::is_nothrow_copy_constructible_v<T>);
  Option(Option&& oth) noexcept(std::is_nothrow_move_constructible_v<T>);

  Option& operator=(const Option& oth) noexcept(std::is_nothrow_copy_constructible_v<T> && std::is_nothrow_copy_assignable_v<T>);
  Option& operator=(Option&& oth) noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_assignable_v<T>);

  explicit operator bool() const noexcept;

  ~Option();

  //*   <--- specialized algorithms & methods  --->
  bool has_value() const noexcept;

  T& value_or(T& val) noexcept;
  const T& value_or(const T& val) const noexcept;
  T value_or_default() noexcept requires std::default_initializable<T>;
  T& value();
  const T& value() const;

  bool operator==(const Option<T>& oth) const noexcept;

  // returns true if the object has been initialized and destroyed:
  bool destroy_value() noexcept;

  //  <--- conversions (cast) : --->
  template <typename E> requires (!std::is_void_v<T> && Error<E>)
  Result<T, E> to_result(E error_if_none);

  //*   <--- functional methods --->
  // Functor interface:
  template <typename Func> requires std::invocable<Func, T>
  auto fmap(Func&& fn)
    noexcept(std::is_nothrow_constructible_v<Option<std::invoke_result_t<Func, T>>> && std::is_nothrow_invocable_v<Func, T>)
    -> Option<std::invoke_result_t<Func, T>>;

  // Applicative interface:
  template<typename Func> requires std::invocable<Func, T>
  auto apply(const Option<Func>& fn_option) const
    noexcept(std::is_nothrow_constructible_v<Option<std::invoke_result_t<Func, T>>> && std::is_nothrow_invocable_v<Func, T>)
    -> Option<std::invoke_result_t<Func, T>>;

  template <typename Func> requires std::invocable<Func, T>
  auto and_then(Func&& fn) const
    noexcept(std::is_nothrow_constructible_v<Option<std::invoke_result_t<Func, T>>> && std::is_nothrow_invocable_v<Func, T>)
    -> std::invoke_result_t<Func, T>;

  template <typename Func, typename... Args> requires std::invocable<Func, Args...>
  auto or_else(Func&& fn, Args&&... args) const
    noexcept(std::is_nothrow_constructible_v<Option<std::invoke_result_t<Func, T>>> && std::is_nothrow_invocable_v<Func, T>)
     -> std::invoke_result_t<Func, T>;

  // coproduction variants (or sum) on the Option<T> monad:
  template <typename U> requires Addable<T, U>
  Option<T>& operator+=(const Option<U>& rhs);

  template <typename U> requires Subtractable<T, U>
  Option<T>& operator-=(const Option<U>& rhs);

  template <typename U> requires Multipliable<T, U>
  Option<T>& operator*=(const Option<U>& rhs);

  template <typename U> requires Dividable<T, U>
  Option<T>& operator/=(const Option<U>& rhs);

private: //* methods:
  void swap(Option& oth) noexcept(std::is_nothrow_swappable_v<T> && std::is_nothrow_move_constructible_v<T>);

private: //* friends:
  template<typename U>
  friend void swap(Option<U>& first, Option<U>& second);

}; // class 'Option';

//* <--- external mnemonic functions --->
template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
Option<T> None() noexcept;

template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
Option<T> Some(T val) noexcept(std::is_nothrow_move_constructible_v<T>);

} // namespace 'tmn::err'

#include "Option.tpp" // for: Option definition;
#include "CoproductOperations.hpp" // for: external monoid functions for Option objects;
//#include "../Result/Result.hpp" // for: implementation of the Result class, which was previously declared forward;


#endif // THROWLESS_OPTION_HPP
