#ifndef TMN_THROWLESS_SHARED_PTR_HPP
#error "Include SharedPtr.hpp instead of SharedPtr.tpp"
#endif

#include "SharedPtr.hpp"

#include <utility>

namespace tmn {

template<typename T>
void SharedPtr<T>::cleanup() noexcept {
  if (control_block) {
    if (control_block->decrement_ref()) {
      delete control_block;
    }
  }
  resource_ptr = nullptr;
  control_block = nullptr;
}

template<typename T>
template<typename U> requires std::is_convertible_v<U*, T*>
void SharedPtr<T>::copy_from(const SharedPtr<U>& oth) noexcept {
  if (oth.control_block) {
    oth.control_block->increment_ref();
  }

  resource_ptr = oth.resource_ptr;
  control_block = oth.control_block;
}

template<typename T>
void SharedPtr<T>::swap(SharedPtr& oth) noexcept {
  std::swap(resource_ptr, oth.resource_ptr);
  std::swap(control_block, oth.control_block);
}

template<typename T>
SharedPtr<T>::SharedPtr(T* ptr) : resource_ptr(ptr) {
  if (ptr) {
    control_block = new ControlBlock<T>(ptr);
  }
}

template<typename T>
template<typename Deleter>
SharedPtr<T>::SharedPtr(T* ptr, Deleter deleter) : resource_ptr(ptr) {
  if (ptr) {
    control_block = new ControlBlock<T>(ptr, std::move(deleter));
  }
}

template<typename T>
SharedPtr<T>::SharedPtr(const SharedPtr& oth) {
  copy_from(oth);
}

template<typename T>
template<typename U> requires std::is_convertible_v<U*, T*>
SharedPtr<T>::SharedPtr(const SharedPtr<U>& oth) noexcept {
  copy_from(oth);
}

template<typename T>
SharedPtr<T>::SharedPtr(SharedPtr&& oth) noexcept
  : resource_ptr(oth.resource_ptr), control_block(oth.control_block)
{
  oth.resource_ptr = nullptr;
  oth.control_block = nullptr;
}

template<typename T>
SharedPtr<T>::~SharedPtr() {
  cleanup();
}

template<typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr& oth) noexcept {
  if (this != &oth) {
      cleanup();
      copy_from(oth);
  }
  return *this;
}

template<typename T>
template<typename U> requires std::is_convertible_v<U*, T*>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<U>& oth) noexcept {
  if (static_cast<const void*>(this) != static_cast<const void*>(&oth)) {
    cleanup();
    copy_from(oth);
  }
  return *this;
}

template<typename T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr&& oth) noexcept {
  if (this != &oth) {
    cleanup();
    resource_ptr = oth.resource_ptr;
    control_block = oth.control_block;
    oth.resource_ptr = nullptr;
    oth.control_block = nullptr;
  }
  return *this;
}

template<typename T>
T* SharedPtr<T>::get() const noexcept {
  return resource_ptr;
}

template<typename T>
Option<T*> SharedPtr<T>::try_get() const noexcept {
  return resource_ptr ? Option<T*>(resource_ptr) : Option<T*>();
}

template<typename T>
T& SharedPtr<T>::operator*() const noexcept {
  assert(resource_ptr != nullptr && "Dereferencing null SharedPtr");
  return *resource_ptr;
}


template<typename T>
T* SharedPtr<T>::operator->() const noexcept {
  // assert(resource_ptr != nullptr && "Attempt to access a null SharedPtr via ->");
  // User should think and process about which pointer the user will receive:
  return resource_ptr;
}

template<typename T>
SharedPtr<T>::operator bool() const noexcept {
  return resource_ptr != nullptr;
}

template<typename T>
size_t SharedPtr<T>::use_count() const noexcept {
  return control_block ? control_block->use_count() : 0;
}

template<typename T>
bool SharedPtr<T>::is_unique() const noexcept {
  return use_count() == 1;
}
template<typename T>
void SharedPtr<T>::reset(T* new_resource_ptr) {
  cleanup();
  if (new_resource_ptr) {
    resource_ptr = new_resource_ptr;
    control_block = new ControlBlock<T>(new_resource_ptr);
  }
}

template<typename T>
template<typename Deleter>
void SharedPtr<T>::reset(T* new_resource_ptr, Deleter deleter) {
  cleanup();
  if (new_resource_ptr) {
    resource_ptr = new_resource_ptr;
    control_block = new ControlBlock<T>(new_resource_ptr, std::move(deleter));
  }
}

template<typename T>
bool SharedPtr<T>::operator==(const SharedPtr& oth) const noexcept {
  return resource_ptr == oth.resource_ptr;
}

template<typename T>
bool SharedPtr<T>::operator!=(const SharedPtr& oth) const noexcept {
  return resource_ptr != oth.resource_ptr;
}

template<typename T>
bool SharedPtr<T>::operator<(const SharedPtr& oth) const noexcept {
  return resource_ptr < oth.resource_ptr;
}

template<typename T>
bool SharedPtr<T>::operator>(const SharedPtr& oth) const noexcept {
  return resource_ptr > oth.resource_ptr;
}

//* Global comparison operators:
template<typename T>
bool operator==(const SharedPtr<T>& lhs, std::nullptr_t) noexcept {
  return !static_cast<bool>(lhs);
}

template<typename T>
bool operator==(std::nullptr_t, const SharedPtr<T>& rhs) noexcept {
  return !static_cast<bool>(rhs);;
}

template<typename T>
bool operator!=(const SharedPtr<T>& lhs, std::nullptr_t) noexcept {
  return static_cast<bool>(lhs);
}

template<typename T>
bool operator!=(std::nullptr_t, const SharedPtr<T>& rhs) noexcept {
  return static_cast<bool>(rhs);
}

} // namespace tmn;
