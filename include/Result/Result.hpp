#ifndef RESULT_H
#define RESULT_H

#include <concepts> // for: std::default_initializable<>;
#include <type_traits> // for: std::conditional;
#include <ostream> // for: std::ostream;
#include <iostream> // for: std::cout;
#include "../Error/ErrorConcept.hpp" // for concept Error;

namespace throwless {

// used in Result<T, E> if T = void
struct EmptyStruct {};

template<typename T, typename E> requires Error<E>
class Result {
private: //* supstructures :
    enum class State {
        Ok,
        Error,
    };

private: //* usings :
    using ValueType = std::conditional_t<std::is_void_v<T>, EmptyStruct, T>;

private: //* fields :
    State state;
    union { ValueType ok_value; E err_value; };
    
public: //* methods :
    //*   <--- static mnemonic methods that call the constructor from an argument --->
    static Result Ok(const ValueType& value) noexcept   requires (!std::is_void_v<T>);
    static Result Ok(ValueType&& value) noexcept        requires (!std::is_void_v<T>);
    static Result Err(const E& error) noexcept;
    static Result Err(E&& error) noexcept;

    //*   <--- constructors, (~)ro5, destructor --->
    Result() noexcept requires std::is_void_v<T>;
    Result(const Result& other) noexcept;
    Result(Result&& other) noexcept;
    Result& operator=(const Result& other) noexcept;
    Result& operator=(Result&& other) noexcept;
    ~Result();

    //*   <--- specialized algorithms & methods  --->
    bool is_ok() const noexcept;
    bool is_err() const noexcept;

    ValueType& unwrap_or(ValueType& default_value) noexcept;
    const ValueType& unwrap_or(const ValueType& default_value) const noexcept;
    T unwrap_or_default() const noexcept requires std::default_initializable<T>;

    E& unwrap_err_or(E& default_err) noexcept;
    const E& unwrap_err_or(const E& default_err) const noexcept;
    E unwrap_err_or_default() const noexcept requires std::default_initializable<E>;

    //*   <--- functional methods (from funcprog)  --->
    // we use the universal reference `Fn&&` (saves the value category (lvalue/rvalue) of the passed object)
    template<typename Fn, typename U = std::invoke_result_t<Fn, T>>
    Result<U, E> map(Fn&& fn) const;
    
    template<typename Fn, typename OthErr = std::invoke_result_t<Fn, E>>
    Result<T, OthErr> map_err(Fn&& fn) const;

    // TODO : and_then, or_else;

    //?   <--- methods for logging the internal state  --->
    #if 0
    Result& log(std::ostream& os = std::cout, const char* message = "") const &;
    Result&& log(std::ostream& os = std::cout, const char* message = "") const &&;
    Result& log_if_err(std::ostream& os = std::cerr, const char* message = "") const &;
    Result&& log_if_err(std::ostream& os = std::cerr, const char* message = "") const &&;
    Result& log_if_ok(std::ostream& os = std::cout, const char* message = "") const &;
    Result&& log_if_ok(std::ostream& os = std::cout, const char* message = "") const &&;
    #endif

    // TODO : implement these methods;

private: //* methods :
    //*   <--- constructors that are called by static methods Ok(value), Err(error) --->
    explicit Result(ValueType&& value) noexcept;
    explicit Result(const ValueType& value) noexcept;
    explicit Result(E&& error) noexcept;
    //explicit Result(const E& error) noexcept;

}; // end of class 'Result'

} // end of namespace 'throwless'

#include "../../src/Result/Result.tpp"

#endif