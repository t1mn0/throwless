#ifndef TMN_THROWLESS_SHARED_PTR_HPP
#error \
"ArrayControlBlock.tpp is a file with hidden implementation details\
of the SmartPtr mechanism and is not intended for use"
#endif

#ifndef TMN_THROWLESS_SHARED_PTR_ARR_CONTROL_BLOCK_HPP
#error "Definition of the class must follow the declaration of the class"
#endif

#include "ArrayControlBlock.hpp"

namespace tmn {

template<typename T>
ControlBlock<T[]>::ControlBlock(T* arr_ptr, size_t size)
  : ref_count(1), weak_count(0), array_ptr(arr_ptr), array_size(size), deleter([](T* p, size_t size) { delete[] p; }) {}

template<typename T>
ControlBlock<T[]>::ControlBlock(T* arr_ptr, size_t size, std::function<void(T*, size_t size)> del)
  : ref_count(1), weak_count(0), array_ptr(arr_ptr), array_size(size), deleter(std::move(del))
{
  if (deleter == nullptr) {
    throw err::NullPtrErr("Deleter<[]>");
  }
}

template<typename T>
void ControlBlock<T[]>::increment_strong() noexcept {
  ref_count.fetch_add(1, std::memory_order_relaxed);
}

template<typename T>
bool ControlBlock<T[]>::decrement_strong() noexcept {
  if (ref_count.fetch_sub(1, std::memory_order_acq_rel) == 1) {
    deleter(array_ptr, array_size);
    array_ptr = nullptr;
    array_size = 0;
    return weak_count.load() == 0;
  }
  return false;
}

template<typename T>
void ControlBlock<T[]>::increment_weak() noexcept {
  weak_count.fetch_add(1, std::memory_order_relaxed);
}

template<typename T>
bool ControlBlock<T[]>::decrement_weak() noexcept {
  return weak_count.fetch_sub(1, std::memory_order_acq_rel) == 1;
}

template<typename T>
size_t ControlBlock<T[]>::counter_value() const noexcept {
  return ref_count.load(std::memory_order_acquire);
}

template<typename T>
size_t ControlBlock<T[]>::size() const noexcept {
  return array_size;
}

} // namespace tmn;
