#ifndef TMN_THROWLESS_OPTION_HPP
#error "Include Option.hpp instead of CoproductOperation.tpp"
#endif

#include <utility> // for: declval;
#include <type_traits> // for: is_integral_v;

#include "Option.hpp"

namespace tmn {

// arithmetic operations (free funcs) that support coproduct for the Option<T> monad
template <typename T, typename U>
requires tmn::arithmetic::Addable<T, U>
auto operator+(const Option<T>& lhs, const Option<U>& rhs)
  -> Option<decltype(std::declval<T>() + std::declval<U>())>
{
  if (lhs.has_value() && rhs.has_value()) {
    return Option<decltype(std::declval<T>() + std::declval<U>())>(
      lhs.value() + rhs.value()
    );
  }
  return Option<decltype(std::declval<T>() + std::declval<U>())>();
}

template <typename T, typename U>
requires tmn::arithmetic::Subtractable<T, U>
auto operator-(const Option<T>& lhs, const Option<U>& rhs)
  -> Option<decltype(std::declval<T>() - std::declval<U>())>
{
  if (lhs.has_value() && rhs.has_value()) {
    return Option<decltype(std::declval<T>() - std::declval<U>())>(
      lhs.value() - rhs.value()
    );
  }
  return Option<decltype(std::declval<T>() - std::declval<U>())>();
}

template <typename T, typename U>
requires tmn::arithmetic::Multipliable<T, U>
auto operator*(const Option<T>& lhs, const Option<U>& rhs)
  -> Option<decltype(std::declval<T>() * std::declval<U>())>
{
  if (lhs.has_value() && rhs.has_value()) {
    return Option<decltype(std::declval<T>() * std::declval<U>())>(
      lhs.value() * rhs.value()
    );
  }
  return Option<decltype(std::declval<T>() * std::declval<U>())>();
}

template <typename T, typename U>
requires tmn::arithmetic::Dividable<T, U>
auto operator/(const Option<T>& lhs, const Option<U>& rhs)
  -> Option<decltype(std::declval<T>() / std::declval<U>())>
{
  if (lhs.has_value() && rhs.has_value()) {
    if constexpr (std::is_integral_v<T> && std::is_integral_v<U>) {
      if (rhs.value() == 0) return Option<decltype(std::declval<T>() / std::declval<U>())>();
    }
    return Option<decltype(std::declval<T>() / std::declval<U>())>(
      lhs.value() / rhs.value()
    );
  }
  return Option<decltype(std::declval<T>() / std::declval<U>())>();
}

} // namespace tmn;
