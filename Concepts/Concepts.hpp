#ifndef TMN_THROWLESS_CONCEPTS_HPP
#define TMN_THROWLESS_CONCEPTS_HPP

#include <type_traits> // for: invoke_result_t;
#include <concepts> // for: same_as;

namespace tmn::err {

//*   <--- Concepts for objects, methods, constraints --->

template <typename T>
concept CopyableOrVoid = std::same_as<T, void> || (std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T>);

template <typename T>
concept MoveableOrVoid = std::same_as<T, void> || (std::is_move_constructible_v<T> && std::is_move_assignable_v<T>);

} // namespace tmn::err;

namespace tmn::func {

//*   <--- Concepts for the general structure of the created functional entities --->

template <template <typename> typename F, typename T, typename Func>
concept Functor = requires(F<T> functor, Func fn) {
  { functor.fmap(fn) } -> std::same_as<F<std::invoke_result_t<Func, T>>>;
};

template <template <typename> typename A, typename T, typename Func>
concept Applicative = requires(A<T> applicative, A<Func> fn_applicative) {
  requires Functor<A, T, Func>;
  // instead of `pure()`, used `default_initializable`:
  requires std::default_initializable<A<T>>;
  { applicative.apply(fn_applicative) } -> std::same_as<A<std::invoke_result_t<Func, T>>>;
};

template <template <typename> typename M, typename T, typename Func>
concept Monad = requires(M<T> monad, Func fn) {
  requires Functor<M, T, Func>;
  // instead of `pure()`, used `default_initializable`:
  requires std::default_initializable<M<T>>;

  // `and_then` synonymous with `bind` or `flatMap` in a stricter functional programming style:
  { monad.and_then(fn) } -> std::same_as<M<std::invoke_result_t<Func, T>>>;
  // ... or we can write that asserts in monad's `.and_then()` implementation:
  // for example Option.and_then():
  // ...
  // static_assert(
  //     is_specialization_of<std::invoke_result_t<Func, T>, Option>::value,
  //     "and_then must return Option<U>"
  // );
};

} // namespace tmn::func;

//*   <--- Concepts for for checking that the template type T supports arithmetic operations --->

namespace tmn::arithmetic{

template <typename T, typename U = T>
concept Addable = requires(T a, U b) {
  { a + b } -> std::convertible_to<T>;
};

template <typename T, typename U = T>
concept Subtractable = requires(T a, U b) {
  { a - b } -> std::convertible_to<T>;
};

template <typename T, typename U = T>
concept Multipliable = requires(T a, U b) {
  { a * b } -> std::convertible_to<T>;
};

template <typename T, typename U = T>
concept Dividable = requires(T a, U b) {
  { a / b } -> std::convertible_to<T>;
  requires !std::is_same_v<U, bool>;
};

} // namespace tmn::arithmetic;


#endif // TMN_THROWLESS_CONCEPTS_HPP;
