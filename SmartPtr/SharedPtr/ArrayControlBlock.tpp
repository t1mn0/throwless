#ifndef TMN_THROWLESS_ARRAY_CONTROL_BLOCK_HPP
#error "Include ArrayControlBlock.hpp instead of ArrayControlBlock.tpp"
#endif

#include "ArrayControlBlock.hpp"

namespace tmn {

template<typename T>
ArrayControlBlock<T>::ArrayControlBlock(T* ptr, size_t size, std::function<void(T*, size_t)> del)
  : ref_count(1), weak_count(0), array_ptr(ptr), array_size(size), deleter(std::move(del))
{
  if (!deleter) {
    deleter = [](T* p, size_t) { delete[] p; };
  }
}

template<typename T>
void ArrayControlBlock<T>::increment_ref() noexcept {
  ref_count.fetch_add(1, std::memory_order_relaxed);
}

template<typename T>
bool ArrayControlBlock<T>::decrement_ref() noexcept {
  if (ref_count.fetch_sub(1, std::memory_order_acq_rel) == 1) {
    deleter(array_ptr, array_size);
    array_ptr = nullptr;
    return decrement_weak();
  }
  return false;
}

template<typename T>
void ArrayControlBlock<T>::increment_weak() noexcept {
  weak_count.fetch_add(1, std::memory_order_relaxed);
}

template<typename T>
bool ArrayControlBlock<T>::decrement_weak() noexcept {
  return weak_count.fetch_sub(1, std::memory_order_acq_rel) == 1;
}

template<typename T>
size_t ArrayControlBlock<T>::use_count() const noexcept {
  return ref_count.load(std::memory_order_acquire);
}

template<typename T>
size_t ArrayControlBlock<T>::size() const noexcept {
  return array_size;
}

} // namespace tmn;
