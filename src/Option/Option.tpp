#ifndef FPP_OPTION_HPP
#error "Include Option.hpp instead of Option.tpp"
#endif

#include "../../include/Option/Option.hpp"

#include <stdexcept> // for: std::runtime_error;
#include <utility> // for: std::move, std::swap;
#include <optional> // for: std::bad_optional_access (exception)

namespace fpp {

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
const T& Option<T>::value_or_exception() const {
    if (!_is_initialized) throw std::bad_optional_access();
    return *reinterpret_cast<const T*>(_value);
}

template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
T& Option<T>::value_or_exception() {
    if (!_is_initialized) throw std::bad_optional_access();
    return *reinterpret_cast<T*>(_value);
}

template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
bool Option<T>::operator==(const Option& oth) const noexcept {
    if (_is_initialized != oth._is_initialized) return false;
    if (!_is_initialized) return true;
    return *reinterpret_cast<T*>(_value) == *reinterpret_cast<T*>(oth._value);
}

template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
Either<T, void> Option<T>::to_either_left() const & {
    if (this->has_value()) {
        return Either<T, void>::from_left(this->value_or_exception());
    }
    return Either<T, void>::from_right();
}

template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
Either<T, void> Option<T>::to_either_left() && {
    if (this->has_value()) {
        return Either<T, void>::from_left(std::move(*this).value_or_exception());
    }
    return Either<T, void>::from_right();
}

template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
Either<void, T> Option<T>::to_either_right() const & {
    if (this->has_value()) {
        return Either<void, T>::from_right(this->value_or_exception());
    }
    return Either<void, T>::from_left();
}

template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
Either<void, T> Option<T>::to_either_right() && {
    if (this->has_value()) {
        return Either<void, T>::from_right(std::move(*this).value_or_exception());
    }
    return Either<void, T>::from_left();
}

template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
template <typename E> requires (!std::is_void_v<T> && Error<E>)
Result<T, E> Option<T>::to_result(E error_if_none) const & {
    if (has_value()) {
        return Result<T, E>::Ok(value_or_exception());
    }
    return Result<T, E>::Err(error_if_none);
}

template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
template <typename E> requires (!std::is_void_v<T> && Error<E>)
Result<T, E> Option<T>::to_result(E error_if_none) && {
    if (has_value()) {
        return Result<T, E>::Ok(std::move(*this).value_or_exception());
    }
    return Result<T, E>::Err(error_if_none);
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

//*   <--- functional methods indicating relationship to the type of func_structure  --->

template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
template <typename Func> requires std::invocable<Func, T>
auto Option<T>::fmap(Func&& fn)
    noexcept(std::is_nothrow_constructible_v<Option<std::invoke_result_t<Func, T>>> && std::is_nothrow_invocable_v<Func, T>)
    -> Option<std::invoke_result_t<Func, T>> 
{
    if (!_is_initialized) return Option<std::invoke_result_t<Func, T>>{};
    return Option<std::invoke_result_t<Func, T>>(fn(*reinterpret_cast<T*>(_value)));
}

template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
template <typename Func> requires std::invocable<Func, T>
auto Option<T>::and_then(Func&& fn) const &
    noexcept(std::is_nothrow_constructible_v<Option<std::invoke_result_t<Func, T>>> && std::is_nothrow_invocable_v<Func, T>)
    -> Option<std::invoke_result_t<Func, T>>
{
    if (!_is_initialized) return Option<std::invoke_result_t<Func, T>>{};
    return Option<std::invoke_result_t<Func, T>>(std::forward<Func>(fn)(*reinterpret_cast<const T*>(_value)));
}

template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
template <typename Func, typename... Args> requires std::invocable<Func, Args...> &&  std::same_as<std::invoke_result_t<Func, Args...>, Option<T>>
Option<T> Option<T>::or_else(Func&& fn, Args&&... args) const & {
    if (_is_initialized) return *this;
    return std::forward<Func>(fn)(std::forward<Args>(args)...);
}

template <typename T> requires (!std::is_void_v<T> && CopyableOrVoid<T> && MoveableOrVoid<T>)
template <typename U> requires Addable<T, U>
Option<T>& Option<T>::operator+=(const Option<U>& rhs) {
    if (has_value() && rhs.has_value()) {
        *reinterpret_cast<T*>(_value) += rhs.value_or_exception();
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
        *reinterpret_cast<T*>(_value) -= rhs.value_or_exception();
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
        *reinterpret_cast<T*>(_value) *= rhs.value_or_exception();
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
        *reinterpret_cast<T*>(_value) /= rhs.value_or_exception();
    } 
    else {
        _is_initialized = false;
    }
    return *this;
}

} // namespace 'fpp'