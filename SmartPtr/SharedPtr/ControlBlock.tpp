#ifndef TMN_THROWLESS_SHARED_PTR_HPP
#error \
"ControlBlock.tpp is a file with hidden implementation details\
of the SmartPtr mechanism and is not intended for use"
#endif

#ifndef TMN_THROWLESS_SHARED_PTR_CONTROL_BLOCK_HPP
#error "Definition of the class must follow the declaration of the class"
#endif

#include "ControlBlock.hpp"
#include "../../Error/Error.hpp"

namespace tmn {

template<typename T>
ControlBlock<T>::ControlBlock(T* ptr)
  : ref_count(1), weak_count(0), object_ptr(ptr), deleter([](T* p) { delete p; }) {}

template<typename T>
ControlBlock<T>::ControlBlock(T* ptr, std::function<void(T*)> del)
  : ref_count(1), weak_count(0), object_ptr(ptr), deleter(std::move(del))
{
  if (deleter == nullptr) {
    throw err::NullPtrErr("Deleter");
  }
}

template<typename T>
void ControlBlock<T>::increment_strong() noexcept {
  ref_count.fetch_add(1, std::memory_order_relaxed);
}

template<typename T>
bool ControlBlock<T>::decrement_strong() noexcept {
  if (ref_count.fetch_sub(1, std::memory_order_acq_rel) == 1) {
    deleter(object_ptr);
    object_ptr = nullptr;
    return weak_count.load() == 0;
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
size_t ControlBlock<T>::counter_value() const noexcept {
  return ref_count.load(std::memory_order_acquire);
}

} // namespace tmn;
