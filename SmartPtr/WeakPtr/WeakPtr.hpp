#ifndef TMN_THROWLESS_WEAK_PTR_HPP
#define TMN_THROWLESS_WEAK_PTR_HPP

#include "../SharedPtr/SharedPtr.hpp"

#include <type_traits>  // for: std::is_convertible_v;

namespace tmn {

// A non-self-contained smart pointer to solve the problems of using SharedPtr:
// to break cyclic references and monitor objects without extending their lifetime
template<typename T>
class WeakPtr {
private:
  ControlBlock<T>* control_block = nullptr;

private:
  void cleanup() noexcept;
  void copy_from(const WeakPtr& oth) noexcept;
  void move_from(WeakPtr&& oth) noexcept;

  void swap(WeakPtr& oth) noexcept;

  friend void swap(WeakPtr<T>& first, WeakPtr<T>& second)
    noexcept(noexcept(first.swap(second)))
  {
    first.swap(second);
  }

public: //* fields:
  //*   <--- constructors, (~)ro5, destructor ... --->
  WeakPtr() noexcept = default;

  WeakPtr(const SharedPtr<T>& shared) noexcept;

  template<typename U> requires std::is_convertible_v<U*, T*>
  WeakPtr(const WeakPtr<U>& oth) noexcept;

  WeakPtr(const WeakPtr& oth) noexcept;
  WeakPtr(WeakPtr&& oth) noexcept;

  WeakPtr& operator=(const SharedPtr<T>& shared) noexcept;

  template<typename U> requires std::is_convertible_v<U*, T*>
  WeakPtr& operator=(const WeakPtr<U>& oth) noexcept;

  WeakPtr& operator=(const WeakPtr& oth) noexcept;
  WeakPtr& operator=(WeakPtr&& oth) noexcept;

  ~WeakPtr();

  // Observers:
  bool is_expired() const noexcept;
  SharedPtr<T> promote() const noexcept;
  size_t counter_value() const noexcept;

  // Modifiers:
  void reset() noexcept;

  // Comparison operators:
  bool operator==(const WeakPtr& oth) const noexcept;
  bool operator!=(const WeakPtr& oth) const noexcept;
};

} // for: namespace tmn;

#include "WeakPtr.tpp"
#include "ArrayWeakPtr.hpp"

#endif // TMN_THROWLESS_WEAK_PTR_HPP
