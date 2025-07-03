#ifndef FPP_OPTION_H
#define FPP_OPTION_H

#include <cstddef> // for: std::byte;
#include <concepts> // for: default_initializable, std::invocable;
#include <type_traits> // for std::is_nothrow_constructible, std::is_void_v
#include "../UsedConcepts/Concepts.hpp"

namespace fpp {

template <typename T> requires (!std::is_void_v<T>)
class Option {
private: //* fields :
    std::byte _value[sizeof(T)];
    bool      _is_initialized;

public: //* methods :
    //*   <--- constructors, (~)ro5, destructor --->
    Option() noexcept;
    explicit Option(const T& val) noexcept(std::is_nothrow_copy_constructible_v<T>);
    explicit Option(T&& val) noexcept(std::is_nothrow_move_constructible_v<T>);
    Option(const Option<T>& oth) noexcept(std::is_nothrow_copy_constructible_v<T>);
    Option(Option<T>&& oth) noexcept(std::is_nothrow_move_constructible_v<T>);
    Option& operator=(const Option<T>& oth) noexcept(std::is_nothrow_copy_constructible_v<T> && std::is_nothrow_copy_assignable_v<T>);
    Option& operator=(Option<T>&& oth) noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_assignable_v<T>);
    void swap(Option<T>& oth) noexcept(std::is_nothrow_swappable_v<T> && std::is_nothrow_move_constructible_v<T>);
    explicit operator bool() const noexcept;
    ~Option();

    //*   <--- specialized algorithms & methods  --->
    bool has_value() const noexcept;

    T& value_or(T& val) noexcept;
    const T& value_or(const T& val) const noexcept;
    T value_or_default() noexcept requires std::default_initializable<T>;
    T& value_or_exception();
    const T& value_or_exception() const;

    bool operator==(const Option& other) const {
        if (_is_initialized != other._is_initialized) return false;
        if (!_is_initialized) return true;
        return *reinterpret_cast<T*>(_value) == *reinterpret_cast<T*>(other._value);
    }

    // returns true if the object has been initialized and destroyed
    bool destroy_value() noexcept;

    //*   <--- functional methods --->
    template <typename Func>
    requires std::invocable<Func, T>
    auto map(Func&& fn)
        noexcept(std::is_nothrow_constructible_v<Option<std::invoke_result_t<Func, T>>> && std::is_nothrow_invocable_v<Func, T>)
        -> Option<std::invoke_result_t<Func, T>>;

    template <typename Func>
    requires std::invocable<Func, T>
    auto and_then(Func&& fn)
        noexcept(std::is_nothrow_constructible_v<Option<std::invoke_result_t<Func, T>>> && std::is_nothrow_invocable_v<Func, T>)
        -> Option<std::invoke_result_t<Func, T>>;

    template <typename Func, typename... Args> 
    requires std::invocable<Func, Args...> && std::same_as<std::invoke_result_t<Func, Args...>, Option<T>>
    Option<T> or_else(Func&& fn, Args&&... args) const &;
    
    template <typename Func, typename... Args>
    requires std::invocable<Func, Args...> &&  std::same_as<std::invoke_result_t<Func, Args...>, Option<T>>
    Option<T> or_else(Func&& fn, Args&&... args) &&;

}; // class 'Option'

} // namespace 'fpp'

#include "../../src/Option/Option.tpp"

// specialization of std::swap delegating to the Option<T> method
namespace std {
    template <typename T> requires (!std::is_void_v<T>)
    void swap(fpp::Option<T>& a, fpp::Option<T>& b) noexcept(noexcept(a.swap(b))) {
        a.swap(b);
    }
}

#endif // FPP_OPTION_H