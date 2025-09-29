#ifndef TMN_THROWLESS_WEAK_PTR_HPP
#error "Definition of the class must follow the declaration of the class"
#endif

#include "WeakPtr.hpp"

namespace tmn {

template<typename T>
void WeakPtr<T[]>::cleanup() noexcept {
  if (control_block != nullptr) {
    control_block->decrement_weak();
    control_block = nullptr;
  }
}

template<typename T>
void WeakPtr<T[]>::copy_from(const WeakPtr& oth) noexcept {
  control_block = oth.control_block;
  if (control_block != nullptr) {
    control_block->increment_weak();
  }
}

template<typename T>
void WeakPtr<T[]>::move_from(WeakPtr&& oth) noexcept {
  control_block = oth.control_block;
  oth.control_block = nullptr;
}

template<typename T>
void WeakPtr<T[]>::swap(WeakPtr& oth) noexcept {
  std::swap(control_block, oth.control_block);
}

template<typename T>
WeakPtr<T[]>::WeakPtr(const SharedPtr<T[]>& shared) noexcept
  : control_block(shared.control_block)
{
  if (control_block != nullptr) {
    control_block->increment_weak();
  }
}

template<typename T>
template<typename U> requires std::is_convertible_v<U(*)[], T(*)[]>
WeakPtr<T[]>::WeakPtr(const WeakPtr<U[]>& oth) noexcept
  : control_block(oth.control_block)
{
  if (control_block != nullptr) {
    control_block->increment_weak();
  }
}

template<typename T>
WeakPtr<T[]>::WeakPtr(const WeakPtr& oth) noexcept {
  copy_from(oth);
}

template<typename T>
WeakPtr<T[]>::WeakPtr(WeakPtr&& oth) noexcept {
  move_from(std::move(oth));
}

template<typename T>
WeakPtr<T[]>& WeakPtr<T[]>::operator=(const SharedPtr<T[]>& shared) noexcept {
  if (control_block != shared.control_block) {
    cleanup();
    control_block = shared.control_block;
    if (control_block != nullptr) {
      control_block->increment_weak();
    }
  }
  return *this;
}

template<typename T>
template<typename U> requires std::is_convertible_v<U(*)[], T(*)[]>
WeakPtr<T[]>& WeakPtr<T[]>::operator=(const WeakPtr<U[]>& oth) noexcept {
  if (control_block != oth.control_block) {
    cleanup();
    control_block = oth.control_block;
    if (control_block != nullptr) {
      control_block->increment_weak();
    }
  }
  return *this;
}

template<typename T>
WeakPtr<T[]>& WeakPtr<T[]>::operator=(const WeakPtr& oth) noexcept {
  if (this != &oth) {
    cleanup();
    copy_from(oth);
  }
  return *this;
}

template<typename T>
WeakPtr<T[]>& WeakPtr<T[]>::operator=(WeakPtr&& oth) noexcept {
  if (this != &oth) {
    cleanup();
    move_from(std::move(oth));
  }
  return *this;
}

template<typename T>
WeakPtr<T[]>::~WeakPtr() {
  cleanup();
}

template<typename T>
bool WeakPtr<T[]>::is_expired() const noexcept {
  return control_block == nullptr || control_block->counter_value() == 0;
}

template<typename T>
SharedPtr<T[]> WeakPtr<T[]>::promote() const noexcept {
  if (control_block != nullptr && control_block->counter_value() > 0) {
    control_block->increment_strong();
    return SharedPtr<T[]>(control_block->array_ptr, control_block->array_size, control_block);
  }
  return SharedPtr<T[]>();
}

template<typename T>
size_t WeakPtr<T[]>::counter_value() const noexcept {
  return control_block != nullptr ? control_block->counter_value() : 0;
}

template<typename T>
size_t WeakPtr<T[]>::size() const noexcept {
  return control_block != nullptr ? control_block->size() : 0;
}

template<typename T>
void WeakPtr<T[]>::reset() noexcept {
  cleanup();
}

template<typename T>
bool WeakPtr<T[]>::operator==(const WeakPtr& oth) const noexcept {
  return control_block == oth.control_block;
}

template<typename T>
bool WeakPtr<T[]>::operator!=(const WeakPtr& oth) const noexcept {
  return control_block != oth.control_block;
}

} // for namespace tmn;
