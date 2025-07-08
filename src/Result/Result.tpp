#include "../../include/Result/Result.hpp"

#ifndef FPP_RESULT_HPP
#error "Include Result.hpp instead of Result.tpp"
#endif

#include <stdexcept> // for: std::runtime_error;
#include <utility> // for: std::move;

namespace fpp {

//*   <--- constructors, (~)ro5, destructor --->

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
Result<T, E>::Result(const T& val) noexcept(std::is_nothrow_copy_constructible_v<T>)
    : state(State::OkState), ok_value(val) {}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
Result<T, E>::Result(T&& val) noexcept(std::is_nothrow_move_constructible_v<T>)
    : state(State::OkState), ok_value(std::move(val)) {}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
Result<T, E>::Result(const E& err) noexcept(std::is_nothrow_copy_constructible_v<E>) 
    : state(State::ErrState), err_value(err) {}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
Result<T, E>::Result(E&& err) noexcept(std::is_nothrow_move_constructible_v<E>)
    : state(State::ErrState), err_value(std::move(err)) {}

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
    return Result(std::move(err));
}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
Result<T,E> Result<T, E>::Err(E&& err) noexcept {
    return Result(err);
}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
Result<T, E>::Result(const Result& oth) 
    noexcept(std::is_nothrow_copy_constructible_v<T> && std::is_nothrow_copy_constructible_v<E>) 
{
    if (oth.is_ok()) {
        ok_value = oth.ok_value;
        state = State::OkState;
    } 
    else {
        err_value = oth.err_value;
        state = State::ErrState;
    }
}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
Result<T, E>::Result(Result&& oth) 
    noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_constructible_v<E>) 
{
    if (oth.is_ok()) {
        // ok_value = std::move(oth.ok_value);
        new (&ok_value) T(std::move(oth.ok_value));
        state = State::OkState;
    } 
    else {
        // err_value = std::move(oth.err_value);
        new (&err_value) E(std::move(oth.err_value));
        state = State::ErrState;
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
        ok_value.~T();
    }
    else if (state == State::ErrState) {
        err_value.~E();
    }

    state = oth.state;
    if (state == State::OkState) {
        ok_value = oth.ok_value;
    } 
    else if (state == State::ErrState) {
        err_value = oth.err_value;
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
        ok_value.~T();
    } 
    else if (state == State::ErrState) {
        err_value.~E();
    }

    state = oth.state;
    if (state == State::OkState) {
        new (&ok_value) T(std::move(oth.ok_value));
    } 
    else if (state == State::ErrState) {
        new (&err_value) E(std::move(oth.err_value));
    }

    return *this;
}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
Result<T, E>::~Result() {
    if (is_ok()) {
        ok_value.~T();
    }
    else {
        err_value.~E();
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
Either<T, E> Result<T, E>::to_either() const & {
    if (is_ok()) {
        return Either<T, E>::from_left(ok_value);
    }
    return Either<T, E>::from_right(err_value);
}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
Either<T, E> Result<T, E>::to_either() && {
    if (is_ok()) {
        return Either<T, E>::from_left(std::move(*this).ok_value);
    }
    return Either<T, E>::from_right(std::move(*this).err_value);
}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
Option<T> Result<T, E>::to_option() const & {
    if (is_ok()) {
        return Option<T>(ok_value);
    }
    return Option<T>();
}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
Option<T> Result<T, E>::to_option() && {
    if (is_ok()) {
        return Option<T>(std::move(*this).ok_value);
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
Option<T> Result<T, E>::unwrap() const noexcept(std::is_nothrow_copy_constructible_v<T>) {
    if (is_ok()) return Option<T>(ok_value);
    return Option<T>();
}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
T& Result<T,E>::unwrap_or(T& val) noexcept(std::is_nothrow_copy_constructible_v<T>) {
    return is_ok() ? ok_value : val;
}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
const T& Result<T,E>::unwrap_or(const T& val) const 
    noexcept(std::is_nothrow_copy_constructible_v<T>) 
{
    return is_ok() ? ok_value : val;
}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
T& Result<T, E>::unwrap_or_exception() {
    if (is_err()) {
        throw std::runtime_error(err_value.err_message());
    }
    return ok_value;
}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
const T& Result<T, E>::unwrap_or_exception() const {
    if (is_err()) {
        throw std::runtime_error(err_value.err_message());
    }
    return ok_value;
}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
T Result<T, E>::unwrap_or_default() const noexcept requires std::default_initializable<T> {
    return is_ok() ? ok_value : T{};
}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
Option<E> Result<T, E>::unwrap_err() const noexcept(std::is_nothrow_copy_constructible_v<E>) {
    if (is_err()) return Option<E>(err_value);
    return Option<E>();
}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
E& Result<T, E>::unwrap_err_or_exception() {
    if (is_ok()) {
        throw std::runtime_error("Result<T, E> does not contain Error");
    }
    return err_value;
}

template <typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
const E& Result<T, E>::unwrap_err_or_exception() const {
    if (is_ok()) {
        throw std::runtime_error("Result<T, E> does not contain Error");
    }
    return err_value;
}   

//*   <--- functional methods  --->

template<typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
template<typename Func> requires std::invocable<Func, T>
auto Result<T, E>::fmap(Func&& fn) const -> Result<std::invoke_result_t<Func, T>, E> {
    using U = std::invoke_result_t<Func, T>;
    
    if (is_err()) {
        return Result<U, E>::Err(err_value);
    }
    try {
        return Result<U, E>::Ok(std::forward<Func>(fn)(ok_value));
    } 
    catch (...) {
        return Result<U, E>::Err(E{"Previous Result::fmap function threw an exception"});
    }
}

template<typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
template<typename Func> requires std::invocable<Func, E>
auto Result<T, E>::fmap_err(Func&& fn) const -> Result<T, std::invoke_result_t<Func, E>> {
    using F = std::invoke_result_t<Func, E>;
    
    if (is_ok()) {
        return Result<T, F>::Ok(ok_value);
    }
    try {
        return Result<T, F>::Err(std::forward<Func>(fn)(err_value));
    } 
    catch (...) {
        return Result<T, F>::Err(F{"Previous Result::map_err function threw an exception"});
    }
}

template<typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
template<typename Func> requires std::invocable<Func, T>
auto Result<T, E>::and_then(Func&& fn) const & -> Result<std::invoke_result_t<Func, T>, E> {
    using U = std::invoke_result_t<Func, T>;
    
    if (is_err()) {
        return Result<U,E>::Err(err_value);
    }
    try {
        return Result<U,E>::Ok(std::forward<Func>(fn)(ok_value));
    } catch (...) {
        return Result<U,E>::Err(E("Previous Result::and_then function threw an exception"));
    }
}

template<typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
template<typename Func, typename... Args> requires std::invocable<Func, E>
auto Result<T, E>::or_else(Func&& fn, Args&&... args) const& -> Result<T, std::invoke_result_t<Func, E>> {
    using U = std::invoke_result_t<Func, E>;
    
    if (is_ok()) {
        return Result<T, U>::Ok(ok_value);
    }
    try {
        return Result<T, U>::Err(std::forward<Func>(fn)(std::forward<Args>(args)...));
    } 
    catch (...) {
        return Result<T, U>::Err(U("or_else function threw an exception"));
    }
}

} // end of namespace 'fpp'
