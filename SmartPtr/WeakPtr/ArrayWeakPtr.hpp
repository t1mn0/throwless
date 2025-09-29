#ifndef TMN_THROWLESS_WEAK_PTR_HPP
#error "Specialization of a class must follow the declaration of the main class template"
#endif

#include "WeakPtr.hpp"

namespace tmn {

template<typename T>
class WeakPtr<T[]> {
private:
  ControlBlock<T[]>* control_block = nullptr;

private:
  void cleanup() noexcept;
  void copy_from(const WeakPtr& oth) noexcept;
  void move_from(WeakPtr&& oth) noexcept;

  void swap(WeakPtr& oth) noexcept;

  friend void swap(WeakPtr<T[]>& first, WeakPtr<T[]>& second)
    noexcept(noexcept(first.swap(second)))
  {
    first.swap(second);
  }

public: //* fields:
  //*   <--- constructors, (~)ro5, destructor ... --->
  WeakPtr() noexcept = default;

  WeakPtr(const SharedPtr<T[]>& shared) noexcept;

  template<typename U> requires std::is_convertible_v<U(*)[], T(*)[]>
  WeakPtr(const WeakPtr<U[]>& oth) noexcept;

  WeakPtr(const WeakPtr& oth) noexcept;
  WeakPtr(WeakPtr&& oth) noexcept;

  WeakPtr& operator=(const SharedPtr<T[]>& shared) noexcept;

  template<typename U> requires std::is_convertible_v<U(*)[], T(*)[]>
  WeakPtr& operator=(const WeakPtr<U[]>& oth) noexcept;

  WeakPtr& operator=(const WeakPtr& oth) noexcept;
  WeakPtr& operator=(WeakPtr&& oth) noexcept;

  ~WeakPtr();

  // Observers (only observation without the possibility of accessing a resource
  // - which corresponds to the idea of WeakPtr):
  bool is_expired() const noexcept;
  SharedPtr<T[]> promote() const noexcept;
  size_t counter_value() const noexcept;
  size_t size() const noexcept;

  // Modifiers:
  void reset() noexcept;

  // Comparison operators:
  bool operator==(const WeakPtr& oth) const noexcept;
  bool operator!=(const WeakPtr& oth) const noexcept;
};

} // for: namespace tmn;

#include "ArrayWeakPtr.tpp"
