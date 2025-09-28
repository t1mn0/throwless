#ifndef TMN_THROWLESS_SHARED_PTR_HPP
#error "Include SharedPtr.hpp instead of ControlBlock.tpp"
#endif

#ifndef TMN_THROWLESS_SHARED_PTR_CONTROL_BLOCK_HPP
#error "Include ControlBlock.hpp instead of ControlBlock.tpp"
#endif

#include "ControlBlock.hpp"

namespace tmn {

template<typename T>
ControlBlock<T>::ControlBlock(T* ptr, std::function<void(T*)> del)
  : ref_count(1), weak_count(0), object_ptr(ptr), deleter(std::move(del))
  {
    if (!deleter) {
      deleter = [](T* p) { delete p; };
    }
  }

template<typename T>
void ControlBlock<T>::increment_ref() noexcept {
  ref_count.fetch_add(1, std::memory_order_relaxed);
}

template<typename T>
bool ControlBlock<T>::decrement_ref() noexcept {
  if (ref_count.fetch_sub(1, std::memory_order_acq_rel) == 1) {
    deleter(object_ptr);
    object_ptr = nullptr;
    return decrement_weak();
  }
  return false;
}

template<typename T>
void ControlBlock<T>::increment_weak() noexcept {
  weak_count.fetch_add(1, std::memory_order_relaxed);
}

template<typename T>
bool ControlBlock<T>::decrement_weak() noexcept {
  return weak_count.fetch_sub(1, std::memory_order_acq_rel) == 1;
}

template<typename T>
size_t ControlBlock<T>::use_count() const noexcept {
  return ref_count.load(std::memory_order_acquire);
}

} // namespace tmn;
