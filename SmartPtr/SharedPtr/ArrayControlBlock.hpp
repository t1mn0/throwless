#ifndef TMN_THROWLESS_SHARED_PTR_HPP
#error \
"ArrayControlBlock.hpp is a file with hidden implementation details\
of the SmartPtr mechanism and is not intended for use"
#endif

#ifndef TMN_THROWLESS_SHARED_PTR_CONTROL_BLOCK_HPP
#error "Specialization of a class must follow the declaration of the main class template"
#endif

#ifndef TMN_THROWLESS_SHARED_PTR_ARR_CONTROL_BLOCK_HPP
#define TMN_THROWLESS_SHARED_PTR_ARR_CONTROL_BLOCK_HPP

#include "ControlBlock.hpp"

namespace tmn {

template<typename T>
struct ControlBlock<T[]> {
private: //* fields:
  std::atomic<size_t> ref_count;
  std::atomic<size_t> weak_count;
  T* array_ptr;
  size_t array_size;
  std::function<void(T*, size_t)> deleter;

private: //* friends:
  template<typename U>
  friend class WeakPtr;

  template<typename U>
  friend class SharedPtr;

public:
  //* constructors:
  ControlBlock(T* arr, size_t size);
  ControlBlock(T* arr, size_t size, std::function<void(T*, size_t size)> del);

  //* modifier-methods:
  void increment_strong() noexcept;
  bool decrement_strong() noexcept;
  void increment_weak() noexcept;
  bool decrement_weak() noexcept;
  size_t counter_value() const noexcept;
  size_t size() const noexcept;
};

} // namespace tmn;

#include "ArrayControlBlock.tpp"

#endif // TMN_THROWLESS_SHARED_PTR_ARR_CONTROL_BLOCK_HPP;
