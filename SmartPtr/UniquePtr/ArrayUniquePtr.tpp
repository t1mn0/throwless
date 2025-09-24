#ifndef TMN_THROWLESS_UNIQUE_PTR_HPP
#error "Include UniquePtr.hpp instead of SpecializedUniquePtr.tpp"
#endif

#include "UniquePtr.hpp"

namespace tmn {

//*   <--- constructors, (~)ro5, destructor --->

template <typename T>
UniquePtr<T[], std::default_delete<T[]>>::UniquePtr() noexcept : array_ptr(nullptr), deleter() {}

template <typename T>
UniquePtr<T[], std::default_delete<T[]>>::UniquePtr(T* ptr) noexcept : array_ptr(ptr), deleter() {}

template <typename T>
UniquePtr<T[], std::default_delete<T[]>>::UniquePtr(UniquePtr&& oth) noexcept
: array_ptr(std::exchange(oth.array_ptr, nullptr)) {}

template <typename T>
auto UniquePtr<T[], std::default_delete<T[]>>::operator=(UniquePtr&& oth) noexcept
  -> UniquePtr<T[], std::default_delete<T[]>>&
{
  if (this != &oth) {
    reset();
    array_ptr = std::exchange(oth.array_ptr, nullptr);
  }
  return *this;
}

template <typename T>
UniquePtr<T[], std::default_delete<T[]>>::~UniquePtr() {
  reset();
}

//*   <--- methods --->

template <typename T>
Option<T*> UniquePtr<T[], std::default_delete<T[]>>::try_get_and_free() noexcept {
  return Option<T*>(std::exchange(array_ptr, nullptr));
}

template <typename T>
T* UniquePtr<T[], std::default_delete<T[]>>::get_and_free() {
  if (!array_ptr) {
    throw std::runtime_error("UniquePtr: no array to get");
  }
  return std::exchange(array_ptr, nullptr);
}

template <typename T>
bool UniquePtr<T[], std::default_delete<T[]>>::set_resource(T* new_resource) noexcept {
  if (has_resource()) {
    return false;
  }
  array_ptr = new_resource;
  return true;
}


template <typename T>
void UniquePtr<T[], std::default_delete<T[]>>::reset(T* new_resource) noexcept {
  T* old_ptr = std::exchange(array_ptr, new_resource);
  if (old_ptr) {
    deleter(old_ptr);
  }
}

template <typename T>
Option<T*> UniquePtr<T[], std::default_delete<T[]>>::try_get() noexcept {
  if (has_resource()) return Option<T*>(array_ptr);
  else return Option<T*>();
}

template <typename T>
T* UniquePtr<T[], std::default_delete<T[]>>::get() {
  if (!array_ptr) {
    throw std::runtime_error("UniquePtr: no array to get");
  }

  return array_ptr;
}

template <typename T>
bool UniquePtr<T[], std::default_delete<T[]>>::has_resource() const noexcept {
  return array_ptr != nullptr;
}

template <typename T>
UniquePtr<T[], std::default_delete<T[]>>::operator bool() const noexcept {
  return has_resource();
}

template <typename T>
bool UniquePtr<T[], std::default_delete<T[]>>::operator==(std::nullptr_t) const noexcept {
  return !has_resource();
}

template <typename T>
bool UniquePtr<T[], std::default_delete<T[]>>::operator!=(std::nullptr_t) const noexcept {
  return has_resource();
}

// <--- array-specific methods --->
template <typename T>
Option<T> UniquePtr<T[], std::default_delete<T[]>>::try_get_val_at(std::size_t index) const noexcept {
  if (!has_resource()) {
    return Option<T>();
  }

  // No bounds checking. User must use valid index.
  // We could add the size field and take into account any work with the array elements.
  // But this contradicts the SRP: Single Responsibility Principle.
  // The UniquePtr is not used to work correctly with an array,
  // it is used for the proper operation of the idea of ownership in C++.
  // I think the most correct implementation in this case is to use a UniquePtr<T[]>
  // and size in the same container, for example, in an Array or Vector.
  return Option<T>(array_ptr[index]);
}

template <typename T>
T& UniquePtr<T[], std::default_delete<T[]>>::operator[](std::size_t index) & {
  if (!has_resource()) {
    throw std::runtime_error("UniquePtr: array is null");
  }

  return array_ptr[index];
}

template <typename T>
const T& UniquePtr<T[], std::default_delete<T[]>>::operator[](std::size_t index) const& {
  if (!has_resource()) {
    throw std::runtime_error("UniquePtr: array is null");
  }
  return array_ptr[index];
}

template <typename T>
void UniquePtr<T[], std::default_delete<T[]>>::swap(UniquePtr& oth) noexcept {
  std::swap(array_ptr, oth.array_ptr);
}

} // for: namepspace tmn;
