#include "../../include/Result/Result.hpp"

#ifndef RESULT_H
#error "Include Result.hpp instead of Result.tpp"
#endif

#include <stdexcept> // for: std::runtime_error;
#include <utility> // for: std::move;

namespace throwless {

//*   <--- constructors, (~)ro5, destructor --->

template <typename T, typename E> requires Error<E>
Result<T, E>::Result() noexcept requires std::is_void_v<T> : state(State::Ok) {}

template <typename T, typename E> requires Error<E>
Result<T, E>::Result(ValueType&& value) noexcept : state(State::Ok), ok_value(std::move(value)) {}

template <typename T, typename E> requires Error<E>
Result<T, E>::Result(E&& error) noexcept : state(State::Error), err_value(std::move(error)) {}

template <typename T, typename E> requires Error<E>
Result<T, E>::Result(const ValueType& value) noexcept : state(State::Ok), ok_value(value) {}

template <typename T, typename E> requires Error<E>
Result<T, E>::Result(const E& error) noexcept : state(State::Error), err_value(error) {}

template <typename T, typename E> requires Error<E>
Result<T,E> Result<T, E>::Ok(const ValueType& value) noexcept requires (!std::is_void_v<T>) {
    return Result(value);
}

template <typename T, typename E> requires Error<E>
Result<T,E> Result<T, E>::Ok(ValueType&& value) noexcept requires (!std::is_void_v<T>) {
    return Result(value);
}

template <typename T, typename E> requires Error<E>
Result<T,E> Result<T, E>::Err(const E& error) noexcept {
    return Result(error);
}

template <typename T, typename E> requires Error<E>
Result<T,E> Result<T, E>::Err(E&& error) noexcept {
    return Result(error);
}

template <typename T, typename E> requires Error<E>
Result<T, E>::Result(const Result& other) noexcept {
    if (other.is_ok()) {
        ok_value = other.ok_value;
    } 
    else {
        err_value = other.err_value;
    }
}

template <typename T, typename E> requires Error<E>
Result<T, E>::Result(Result&& other) noexcept {
    if (other.is_ok()) {
        ok_value = std::move(other.ok_value);
    } 
    else {
        err_value = std::move(other.err_value);
    }
}

template <typename T, typename E> requires Error<E>
Result<T,E>& Result<T, E>::operator=(const Result<T,E>& other) noexcept {
    if (this == &other) {
        return *this;
    }

    if (state == State::Ok) {
        ok_value.~T();
    }
    else if (state == State::Error) {
        err_value.~E();
    }

    state = other.state;
    if (state == State::Ok) {
        ok_value = other.ok_value;
    } 
    else if (state == State::Error) {
        err_value = other.err_value;
    }

    return *this;
}

template <typename T, typename E> requires Error<E>
Result<T,E>& Result<T,E>::operator=(Result<T,E>&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    if (state == State::Ok) {
        ok_value.~T();
    } 
    else if (state == State::Error) {
        err_value.~E();
    }

    state = other.state;
    if (state == State::Ok) {
        ok_value = other.ok_value;
    } 
    else if (state == State::Error) {
        err_value = other.err_value;
    }

    return *this;
}

template <typename T, typename E> requires Error<E>
Result<T, E>::~Result() {
    if (std::is_void_v<T>) return;

    if (is_ok()) {
        ok_value.~ValueType();
    }
    else {
        err_value.~E();
    }
}

//*   <--- specialized algorithms & methods  --->

template <typename T, typename E> requires Error<E>
bool Result<T,E>::is_ok() const noexcept { 
    return state == State::Ok; 
}

template <typename T, typename E> requires Error<E>
bool Result<T,E>::is_err() const noexcept { 
    return state == State::Error; 
}

template <typename T, typename E> requires Error<E>
Result<T,E>::ValueType& Result<T,E>::unwrap_or(ValueType& default_value) noexcept {
    return is_ok() ? ok_value : default_value;
}

template <typename T, typename E> requires Error<E>
const Result<T,E>::ValueType& Result<T,E>::unwrap_or(const ValueType& default_value) const noexcept {
    return is_ok() ? ok_value : default_value;
}

template <typename T, typename E> requires Error<E>
T Result<T, E>::unwrap_or_default() const noexcept requires std::default_initializable<T> {
    return is_ok() ? ok_value : T{};
}

template <typename T, typename E> requires Error<E>
E& Result<T, E>::unwrap_err_or(E& default_err) noexcept {
    return is_err() ? err_value : default_err;
}

template <typename T, typename E> requires Error<E>
const E& Result<T, E>::unwrap_err_or(const E& default_err) const noexcept {
    return is_err() ? err_value : default_err;
}

template <typename T, typename E> requires Error<E>
E Result<T, E>::unwrap_err_or_default() const noexcept requires std::default_initializable<E> {
    return is_err() ? err_value : E{};
}

//*   <--- functional methods (from funcprog)  --->

template <typename T, typename E> requires Error<E>
template<typename Fn, typename U>
Result<U, E> Result<T, E>::map(Fn&& fn) const {
    if (is_ok()) {
        return Result<U, E>(std::forward<Fn>(fn)(ok_value));
    } else {
        return Result<U, E>(err_value);
    }
}

template <typename T, typename E> requires Error<E>
template<typename Fn, typename OthErr>
Result<T, OthErr> Result<T, E>::map_err(Fn&& fn) const {
    if (is_err()) {
        return Result<T, OthErr>(std::forward<Fn>(fn)(err_value));
    } else {
        return Result<T, OthErr>(ok_value);
    }
}

#if 0
template <typename T, typename E> requires Error<E>
template<typename Fn, typename U>
Result<U, E> Result<T, E>::or_else(Fn&& fn) const {
    if (is_err()) {
        return Result<U, E>(std::forward<Fn>(fn)(err_value));
    } else {
        return Result<U, E>(ok_value);
    }
}
#endif

} // end of nam
