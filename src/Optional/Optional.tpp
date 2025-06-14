#include "../../include/Optional/Optional.hpp"

#ifndef OPTIONAL_H
#error "Include Optional.hpp instead of Optional.tpp"
#endif

#include <stdexcept> // for: std::runtime_error;
#include <utility> // for: std::move, std::swap;

namespace throwless {

//*   <--- constructors, (~)ro5, destructor --->

template <typename T> requires (!std::is_void_v<T>)
Optional<T>::Optional() noexcept : _is_initialized(false) {}

template <typename T> requires (!std::is_void_v<T>)
Optional<T>::Optional(const T& val) noexcept : _is_initialized(true) {
    new (_value) T(val);
}

template <typename T> requires (!std::is_void_v<T>)
Optional<T>::Optional(T&& val) noexcept : _is_initialized(true) {
    new (_value) T(val);
}

template <typename T> requires (!std::is_void_v<T>)
Optional<T>::Optional(const Optional<T>& oth) noexcept : _is_initialized(oth._is_initialized) {
    if (_is_initialized) {
        new (_value) T(*reinterpret_cast<T*>(oth._value));
    }
}

template <typename T> requires (!std::is_void_v<T>)
Optional<T>::Optional(Optional<T>&& oth) noexcept : _is_initialized(oth._is_initialized) {
    if (_is_initialized) {
        new (_value) T(std::move(*reinterpret_cast<T*>(oth._value)));
        reinterpret_cast<T*>(oth._value)->~T();
        oth._is_initialized = false;
    }
}

template <typename T> requires (!std::is_void_v<T>)
Optional<T>& Optional<T>::operator=(const Optional<T>& oth) noexcept {
    Optional tmp(oth);
    swap_with(tmp);
    return *this;
}

template <typename T> requires (!std::is_void_v<T>)
Optional<T>& Optional<T>::operator=(Optional<T>&& oth) noexcept {
    _is_initialized = oth._is_initialized;
    if (_is_initialized) {
        new (_value) T(std::move(*reinterpret_cast<T*>(oth._value)));
        reinterpret_cast<T*>(oth._value)->~T();
        oth._is_initialized = false;
    }
    return *this;
}

template <typename T> requires (!std::is_void_v<T>)
void Optional<T>::swap_with(Optional<T>& oth) noexcept {
    std::swap(_value, oth._value);
    std::swap(_is_initialized, oth._is_initialized);
}

template <typename T> requires (!std::is_void_v<T>)
Optional<T>::operator bool() const noexcept {
    return _is_initialized;
}

template <typename T> requires (!std::is_void_v<T>)
Optional<T>::~Optional(){
    if (_is_initialized){
        reinterpret_cast<T*>(_value)->~T();
    }
    _is_initialized = false;
}

//*   <--- specialized algorithms & methods  --->

template <typename T> requires (!std::is_void_v<T>)
bool Optional<T>::has_value() const noexcept {
    return _is_initialized;
}

template <typename T> requires (!std::is_void_v<T>)
T& Optional<T>::value_or(T& val) noexcept {
    return _is_initialized ? *reinterpret_cast<T*>(_value) : val;
}

template <typename T> requires (!std::is_void_v<T>)
const T& Optional<T>::value_or(const T& val) const noexcept {
    return _is_initialized ? *reinterpret_cast<const T*>(_value) : val;
}

template <typename T> requires (!std::is_void_v<T>)
T Optional<T>::value_or_default() const noexcept requires std::default_initializable<T> {
    return _is_initialized ? *reinterpret_cast<const T*>(_value) : T{};
}

} // end of namespace 'throwless'