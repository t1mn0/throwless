#ifndef FPP_RESULT_HPP
#define FPP_RESULT_HPP

#include <concepts> // for: std::default_initializable<>;
#include <type_traits> // for: std::conditional, std::is_nothrow_constructible;
#include "../Option/Option.hpp"
#include "../Either/Either.hpp"
#include "../Error/ErrorConcept.hpp"

namespace fpp {

template<typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
class Result {
private: //* substructures :
    enum class State {
        OkState,
        ErrState,
    };

private: //* fields :
    State state;
    
    union {
        T ok_value;
        E err_value;
    };
    
public:
    //*   <--- constructors, (~)ro5, destructor --->
    Result() = delete;
    Result(const Result& oth) noexcept(std::is_nothrow_copy_constructible_v<T> && std::is_nothrow_copy_constructible_v<E>);
    Result(Result&& oth) noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_constructible_v<E>);
    Result& operator=(const Result& oth) noexcept(std::is_nothrow_copy_constructible_v<T> && std::is_nothrow_copy_constructible_v<E>);
    Result& operator=(Result&& oth) noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_constructible_v<E>);
    ~Result();

    void swap(Result<T,E>& oth) 
        noexcept(std::is_nothrow_swappable_v<T> && std::is_nothrow_move_constructible_v<T> &&
                 std::is_nothrow_swappable_v<E> && std::is_nothrow_move_constructible_v<E>);

    // Conversions (cast) :
    explicit operator bool() const noexcept;

    Either<T, E> to_either() const &;
    Either<T, E> to_either() &&;
    
    Option<T> to_option() const &;
    Option<T> to_option() &&;

    explicit operator Either<T, E>() const & { return to_either(); }
    explicit operator Either<T, E>() && { return std::move(*this).to_either(); }
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

    Option<T> unwrap() const noexcept(std::is_nothrow_copy_constructible_v<T>);
    T& unwrap_or(T& val) noexcept(std::is_nothrow_copy_constructible_v<T>);
    const T& unwrap_or(const T& val) const noexcept(std::is_nothrow_copy_constructible_v<T>);
    T& unwrap_or_exception();
    const T& unwrap_or_exception() const;
    T unwrap_or_default() const noexcept requires std::default_initializable<T>;

    Option<E> unwrap_err() const noexcept(std::is_nothrow_copy_constructible_v<E>);
    E& unwrap_err_or_exception();
    const E& unwrap_err_or_exception() const;

    //*   <--- functional methods (from funcprog)  --->
    template<typename Func> requires std::invocable<Func, T>
    auto fmap(Func&& fn) const -> Result<std::invoke_result_t<Func, T>, E>;
    
    template<typename Func> requires std::invocable<Func, E>
    auto fmap_err(Func&& fn) const -> Result<T, std::invoke_result_t<Func, E>>;
    
    template<typename Func> requires std::invocable<Func, T>
    auto and_then(Func&& fn) const& -> Result<std::invoke_result_t<Func, T>, E>;

    template<typename Func, typename... Args> requires std::invocable<Func, E>
    auto or_else(Func&& fn, Args&&... args) const& -> Result<T, std::invoke_result_t<Func, E>>;

private:
    //*   <--- constructors that are called by static methods Ok(val), Err(error) --->
    explicit Result(const T& val) noexcept(std::is_nothrow_copy_constructible_v<T>);
    explicit Result(T&& val) noexcept(std::is_nothrow_move_constructible_v<T>);
    explicit Result(const E& error) noexcept(std::is_nothrow_copy_constructible_v<E>);
    explicit Result(E&& error) noexcept(std::is_nothrow_move_constructible_v<E>);

}; // class 'Result'

} // namespace 'fpp'

#include "../../src/Result/Result.tpp"
#include "../Error/Error.hpp"

#endif // FPP_RESULT_HPP