#ifndef TMN_THROWLESS_SHARED_PTR_HPP
#error "Include SharedPtr.hpp instead of ArrayControlBlock.hpp"
#endif

#ifndef TMN_THROWLESS_ARRAY_SHARED_PTR_CONTROL_BLOCK_HPP
#define TMN_THROWLESS_ARRAY_SHARED_PTR_CONTROL_BLOCK_HPP

#include <atomic>
#include <functional>

namespace tmn {

template<typename T>
struct ArrayControlBlock {
  std::atomic<size_t> ref_count;
  std::atomic<size_t> weak_count;
  T* array_ptr;
  size_t array_size;
  std::function<void(T*, size_t)> deleter;

  ArrayControlBlock(T* ptr, size_t size, std::function<void(T*, size_t)> del = nullptr);

  void increment_ref() noexcept;
  bool decrement_ref() noexcept;
  void increment_weak() noexcept;
  bool decrement_weak() noexcept;
  size_t use_count() const noexcept;
  size_t size() const noexcept;
};

} // namespace tmn;

#include "ArrayControlBlock.tpp"

#endif // TMN_THROWLESS_ARRAY_SHARED_PTR_CONTROL_BLOCK_HPP
