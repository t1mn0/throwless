#ifndef FPP_USED_CONCEPTS_H
#define FPP_USED_CONCEPTS_H

#include <type_traits> // for std::invoke_result_t
#include <functional> // for std::function

namespace fpp {

//*   <--- Concepts for objects, methods, constraints

template <typename T>
concept CopyableOrVoid = std::same_as<T, void> || std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T>;

template <typename T>
concept MoveableOrVoid = std::same_as<T, void> || std::is_move_constructible_v<T> && std::is_move_assignable_v<T>;

//*   <--- Concepts for for the general structure of the created functional entities

template <template <typename> typename F, typename T, typename Func>
concept Functor = requires(F<T> functor, Func fn) {
    { functor.fmap(fn) } -> std::same_as<F<std::invoke_result_t<Func, T>>>;
};

#if 0
template <template <typename> typename M, typename T>
concept Monad = ...

template <typename M>
concept Monoid = ...
#endif

//*   <--- Concepts for for checking that the template type T supports arithmetic operations

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

} // namespace 'fpp'


#endif // FPP_USED_CONCEPTS_H