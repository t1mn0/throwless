#ifndef FPP_OPTION_HPP
#define FPP_OPTION_HPP

#include <concepts> // for: default_initializable, std::invocable;
#include <type_traits> // for std::is_nothrow_constructible, std::is_void_v
#include "../UsedConcepts/Concepts.hpp"
#include "../Error/ErrorConcept.hpp"

namespace fpp {

// forward-declarations:
template <typename L, typename R> 
requires (!std::is_same_v<L, R> && CopyableOrVoid<L> && CopyableOrVoid<R> && MoveableOrVoid<L> && MoveableOrVoid<R>)
class Either;

template<typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
class Result;

template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
class Option {
private: //* fields :
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

    void swap(Option& oth) noexcept(std::is_nothrow_swappable_v<T> && std::is_nothrow_move_constructible_v<T>);
    
    explicit operator bool() const noexcept;

    ~Option();

    //*   <--- specialized algorithms & methods  --->
    bool has_value() const noexcept;

    T& value_or(T& val) noexcept;
    const T& value_or(const T& val) const noexcept;
    T value_or_default() noexcept requires std::default_initializable<T>;
    T& value_or_exception();
    const T& value_or_exception() const;

    bool operator==(const Option<T>& oth) const noexcept;

    // Conversions (cast) :
    Either<T, void> to_either_left() const &;
    Either<T, void> to_either_left() &&;
    
    Either<void, T> to_either_right() const &;
    Either<void, T> to_either_right() &&;

    template <typename E> requires (!std::is_void_v<T> && Error<E>)
    Result<T, E> to_result(E error_if_none) const &;
    
    template <typename E> requires (!std::is_void_v<T> && Error<E>)
    Result<T, E> to_result(E error_if_none) &&;

    // returns true if the object has been initialized and destroyed
    bool destroy_value() noexcept;

    //*   <--- functional methods --->
    template <typename Func> requires std::invocable<Func, T>
    auto fmap(Func&& fn)
        noexcept(std::is_nothrow_constructible_v<Option<std::invoke_result_t<Func, T>>> && std::is_nothrow_invocable_v<Func, T>)
        -> Option<std::invoke_result_t<Func, T>>;
        
    template <typename Func> requires std::invocable<Func, T>
    auto and_then(Func&& fn) const &
        noexcept(std::is_nothrow_constructible_v<Option<std::invoke_result_t<Func, T>>> && std::is_nothrow_invocable_v<Func, T>)
        -> Option<std::invoke_result_t<Func, T>>;

    template <typename Func, typename... Args> 
    requires std::invocable<Func, Args...> && std::same_as<std::invoke_result_t<Func, Args...>, Option<T>>
    Option<T> or_else(Func&& fn, Args&&... args) const &;

    // coproduction variants (or sum) on the Option<T> monad
    template <typename U> requires Addable<T, U>
    Option<T>& operator+=(const Option<U>& rhs);

    template <typename U> requires Subtractable<T, U>
    Option<T>& operator-=(const Option<U>& rhs);

    template <typename U> requires Multipliable<T, U>
    Option<T>& operator*=(const Option<U>& rhs);

    template <typename U> requires Dividable<T, U>
    Option<T>& operator/=(const Option<U>& rhs);

private: //* friends :
    template <typename L, typename R> 
    requires (!std::is_same_v<L, R> && CopyableOrVoid<L> && CopyableOrVoid<R> && MoveableOrVoid<L> && MoveableOrVoid<R>)
    friend class Either;

}; // class 'Option'

} // namespace 'fpp'

#include "CoproductOperations.hpp"
#include "../../src/Option/Option.tpp"
#include "../Either/Either.hpp"
#include "../Result/Result.hpp"
#include "../Error/Error.hpp"

// specialization of std::swap delegating to the Option<T> method
namespace std {

template <typename T> requires (!std::is_void_v<T>)
void swap(fpp::Option<T>& a, fpp::Option<T>& b) noexcept(noexcept(a.swap(b))) {
    a.swap(b);
}

} // namespace 'std'

#endif // FPP_OPTION_HPP