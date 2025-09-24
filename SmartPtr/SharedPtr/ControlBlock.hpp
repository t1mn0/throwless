#ifndef TMN_THROWLESS_CONTROL_BLOCK_HPP
#define TMN_THROWLESS_CONTROL_BLOCK_HPP

#include <atomic>
#include <functional>

namespace tmn {

template<typename T>
struct ControlBlock {
  //* fields:
  std::atomic<size_t> ref_count;
  std::atomic<size_t> weak_count;
  T* object_ptr;
  std::function<void(T*)> deleter;

  //* constructor:
  ControlBlock(T* ptr, std::function<void(T*)> del = nullptr);

  //* methods:
  void increment_ref() noexcept;
  bool decrement_ref() noexcept;
  void increment_weak() noexcept;
  bool decrement_weak() noexcept;
  size_t use_count() const noexcept;
};

} // namespace tmn;

#include "ControlBlock.tpp"

#endif // TMN_THROWLESS_CONTROL_BLOCK_HPP;
