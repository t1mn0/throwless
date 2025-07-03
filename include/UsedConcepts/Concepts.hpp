#ifndef FPP_USED_CONCEPTS_H
#define FPP_USED_CONCEPTS_H

#include <type_traits> // for std::invoke_result_t
#include <functional> // for std::function

namespace fpp {

//*   <--- Concepts for objects, methods, constraints

template <typename T>
concept Copyable = std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T>;

template <typename T>
concept Moveable = std::is_move_constructible_v<T> && std::is_move_assignable_v<T>;

//*   <--- Concepts for for the general structure of the created functional entities

template <template <typename> typename F, typename T, typename Func>
concept Functor = requires(F<T> functor, Func fn) {
    { functor.map(fn) } -> std::same_as<F<std::invoke_result_t<Func, T>>>;
};

#if 0
template <template <typename> typename M, typename T>
concept Monad = ...

template <typename M>
concept Monoid = ...
#endif

} // namespace 'fpp'


#endif // FPP_USED_CONCEPTS_H