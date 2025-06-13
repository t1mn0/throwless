#ifndef ERROR_H
#define ERROR_H

#include <cstddef> // for: std::byte;

namespace throwless {

template <typename T>
class Optional {
private: //* fields :
    std::byte _value[sizeof(T)];
    bool      _is_initialized;

public: //* methods :
    //*   <--- constructors, (~)ro5, destructor --->
    Optional() noexcept;
    explicit Optional(const T& val) noexcept;
    explicit Optional(T&& val) noexcept;
    Optional(const Optional<T>& oth) noexcept;
    Optional(Optional<T>&& oth) noexcept;
    Optional& operator=(const Optional& oth) noexcept;
    Optional& operator=(Optional&& oth) noexcept;
    void swap(Optional<T>& oth) noexcept;
    explicit operator bool() const noexcept;
    ~Optional();

    //*   <--- specialized algorithms & methods  --->
    bool has_value() const noexcept;

    T& value_or(T& val) const noexcept;
    const T& value_or(const T& val) const noexcept;
    T value_or_default() const noexcept requires std::default_initializable<T>;

    // TODO : funcprog methods;
    
}; // end of class 'Optional'

} // end of namespace 'throwless'

#include "../../src/Optional/Optional.tpp"

#endif