#ifndef THROWLESS_RESULT_HPP
#define THROWLESS_RESULT_HPP

#include <concepts> // for: default_initializable<>;
#include <type_traits> // for: conditional, is_nothrow_constructible;

#include "../Option/Option.hpp" // for: class conversion;
#include "../Error/ErrorConcept.hpp" // for: Error<E> concept;

namespace tmn::err {

template<typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
class Result {
private: //* substructures:
  enum class State {
    OkState,
    ErrState,
    Uninitialized
  };

private: //* fields:
  State state;

  union {
    T ok_val;
    E err_val;
  };

public: //* methods:
  //*   <--- constructors, (~)ro5, destructor --->
  Result() = delete;
  Result(const Result& oth) noexcept(std::is_nothrow_copy_constructible_v<T> && std::is_nothrow_copy_constructible_v<E>);
  Result(Result&& oth) noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_constructible_v<E>);
  Result& operator=(const Result& oth) noexcept(std::is_nothrow_copy_constructible_v<T> && std::is_nothrow_copy_constructible_v<E>);
  Result& operator=(Result&& oth) noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_constructible_v<E>);
  ~Result();

  //*   <--- constructors that are called by static methods Ok(val), Err(error) --->
  Result(const T& val) noexcept(std::is_nothrow_copy_constructible_v<T>);
  Result(T&& val) noexcept(std::is_nothrow_move_constructible_v<T>);
  Result(const E& err) noexcept(std::is_nothrow_copy_constructible_v<E>);
  Result(E&& err) noexcept(std::is_nothrow_move_constructible_v<E>);

  // Conversions (cast) :
  // true if ok_val in union; false if err_val;
  explicit operator bool() const noexcept;

  Option<T> to_option();

  explicit operator Option<T>() const & { return to_option(); }
  explicit operator Option<T>() && { return std::move(*this).to_option(); }

  //*   <--- static mnemonic methods that call the constructor from an argument --->
  static Result Ok(const T& val) noexcept   requires (!std::is_void_v<T>);
  static Result Ok(T&& val) noexcept        requires (!std::is_void_v<T>);
  static Result Err(const E& error) noexcept;
  static Result Err(E&& error) noexcept;

  //*   <--- specialized algorithms & methods  --->
  bool is_ok() const noexcept;
  bool is_err() const noexcept;

  Option<T> optional_value() const noexcept(std::is_nothrow_copy_constructible_v<T>);
  T& value_or(T& val) noexcept(std::is_nothrow_copy_constructible_v<T>);
  const T& value_or(const T& val) const noexcept(std::is_nothrow_copy_constructible_v<T>);
  T& value();
  const T& value() const;
  T value_or_default() const noexcept requires std::default_initializable<T>;

  Option<E> optional_err() const noexcept(std::is_nothrow_copy_constructible_v<E>);
  E& err();
  const E& err() const;

  //*   <--- functional methods (from funcprog)  --->
  // !!! Result object can have one of two values.
  // Therefore, the passed function (fn) must be able to handle both options:
  // for this, it can, for example, be overloaded:
  template <typename Func> requires std::invocable<Func, T>
  auto fmap(Func&& fn)
    noexcept(std::is_nothrow_constructible_v<Result<std::invoke_result_t<Func, T>, E>> && std::is_nothrow_invocable_v<Func, T>)
    -> Result<std::invoke_result_t<Func, T>, E>;

  template <typename Func> requires std::invocable<Func, E>
  auto fmap_err(Func&& fn) const
    noexcept(std::is_nothrow_constructible_v<Result<T, std::invoke_result_t<Func, E>>> && std::is_nothrow_invocable_v<Func, T>)
    -> Result<T, std::invoke_result_t<Func, E>>;

  template <typename Func>
  requires std::invocable<Func, T> && std::same_as<std::invoke_result_t<Func, T>, Result<std::invoke_result_t<Func, T>, E>>
  auto and_then(Func&& fn) const
    noexcept(std::is_nothrow_constructible_v<Result<std::invoke_result_t<Func, T>, E>> && std::is_nothrow_invocable_v<Func, T>)
    -> std::invoke_result_t<Func, T>;

private: //* methods:
  void swap(Result<T,E>& oth)
    noexcept(std::is_nothrow_swappable_v<T> && std::is_nothrow_move_constructible_v<T> &&
    std::is_nothrow_swappable_v<E> && std::is_nothrow_move_constructible_v<E>);

}; // class 'Result';

} // for: namespace tmn::err;

#include "Result.tpp" // for: Result definition;

#endif // THROWLESS_RESULT_HPP
