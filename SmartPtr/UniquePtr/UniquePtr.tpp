#ifndef TMN_THROWLESS_UNIQUE_PTR_HPP
#error "Include UniquePtr.hpp instead of GeneralUniquePtr.tpp"
#endif

#include "UniquePtr.hpp"

namespace tmn {

//*   <--- constructors, (~)ro5, destructor --->

template <typename T, typename Deleter>
UniquePtr<T, Deleter>::UniquePtr() noexcept : resource_ptr(nullptr), deleter() {}

template <typename T, typename Deleter>
UniquePtr<T, Deleter>::UniquePtr(T* ptr) noexcept : resource_ptr(ptr), deleter() {}

template <typename T, typename Deleter>
template <typename U> requires (std::is_convertible_v<U*, T*>)
UniquePtr<T, Deleter>::UniquePtr(U* ptr) noexcept : resource_ptr(ptr), deleter() {}

template <typename T, typename Deleter>
UniquePtr<T, Deleter>::UniquePtr(UniquePtr&& oth) noexcept : resource_ptr(oth.resource_ptr), deleter(std::move(oth.deleter)) {
  oth.resource_ptr = nullptr;
}

template <typename T, typename Deleter>
UniquePtr<T, Deleter>& UniquePtr<T, Deleter>::operator=(UniquePtr&& oth) noexcept {
  if (this != &oth) {
    reset();
    resource_ptr = std::exchange(oth.resource_ptr, nullptr);
    deleter = std::move(oth.deleter);
  }
  return *this;
}

template <typename T, typename Deleter>
UniquePtr<T, Deleter>::~UniquePtr() {
  reset();
}

//*   <--- methods  --->

template <typename T, typename Deleter>
Option<T*> UniquePtr<T, Deleter>::try_get_and_free() noexcept {
  if (has_resource()) {
    T* temp = resource_ptr;
    resource_ptr = nullptr;
    return Option<T*>(temp);
  }
  return Option<T*>();
}

template <typename T, typename Deleter>
T* UniquePtr<T, Deleter>::get_and_free() {
  assert(has_resource() && "UniquePtr: no resource to get");
  T* temp = resource_ptr;
  resource_ptr = nullptr;
  return temp;
}

template <typename T, typename Deleter>
bool UniquePtr<T, Deleter>::set_resource(T* new_resource) noexcept {
  if (has_resource()) {
    return false;
  }

  resource_ptr = new_resource;
  return true;
}

template <typename T, typename Deleter>
void UniquePtr<T, Deleter>::reset(T* new_resource) noexcept {
  T* old_ptr = std::exchange(resource_ptr, new_resource);
  if (old_ptr) {
    deleter(old_ptr);
  }
}

template <typename T, typename Deleter>
Option<T*> UniquePtr<T, Deleter>::try_get() noexcept {
  if (has_resource()) return Option<T*>(resource_ptr);
  return Option<T*>();
}

template <typename T, typename Deleter>
T* UniquePtr<T, Deleter>::get() {
  assert(has_resource() && "UniquePtr: no resource to get");
  return resource_ptr;
}

template <typename T, typename Deleter>
bool UniquePtr<T, Deleter>::has_resource() const noexcept {
  return resource_ptr != nullptr;
}

template <typename T, typename Deleter>
UniquePtr<T, Deleter>::operator bool() const noexcept {
  return has_resource();
}

template <typename T, typename Deleter>
bool UniquePtr<T, Deleter>::operator==(std::nullptr_t) const noexcept {
  return !has_resource();
}

template <typename T, typename Deleter>
bool UniquePtr<T, Deleter>::operator!=(std::nullptr_t) const noexcept {
  return has_resource();
}

template <typename T, typename Deleter>
Option<T> UniquePtr<T, Deleter>::try_get_val() const noexcept {
  if (has_resource()) {
    return Option<T>(*resource_ptr);
  }
  return Option<T>();
}

template <typename T, typename Deleter>
T& UniquePtr<T, Deleter>::operator*() & {
  assert(has_resource() && "UniquePtr: no resource to get");
  return *resource_ptr;
}

template <typename T, typename Deleter>
const T& UniquePtr<T, Deleter>::operator*() const & {
  assert(has_resource() && "UniquePtr: no resource to get");
  return *resource_ptr;
}

template <typename T, typename Deleter>
void UniquePtr<T, Deleter>::swap(UniquePtr& oth) noexcept {
  std::swap(resource_ptr, oth.resource_ptr);
  std::swap(deleter, oth.deleter);
}

template <typename T, typename Deleter>
bool UniquePtr<T, Deleter>::operator==(const UniquePtr& oth) const noexcept {
  return resource_ptr == oth.resource_ptr;
}

template <typename T, typename Deleter>
bool UniquePtr<T, Deleter>::operator!=(const UniquePtr& oth) const noexcept {
  return resource_ptr != oth.resource_ptr;
}

template <typename T, typename Deleter>
bool UniquePtr<T, Deleter>::operator<(const UniquePtr& oth) const noexcept {
  return resource_ptr < oth.resource_ptr;
}

template <typename T, typename Deleter>
bool UniquePtr<T, Deleter>::operator>(const UniquePtr& oth) const noexcept {
  return resource_ptr > oth.resource_ptr;
}

//* Global comparison operators:
template <typename T, typename Deleter>
bool operator==(const UniquePtr<T, Deleter>& lhs, std::nullptr_t) noexcept {
  return !static_cast<bool>(lhs);
}

template <typename T, typename Deleter>
bool operator==(std::nullptr_t, const UniquePtr<T, Deleter>& rhs) noexcept {
  return !static_cast<bool>(rhs);;
}

template <typename T, typename Deleter>
bool operator!=(const UniquePtr<T, Deleter>& lhs, std::nullptr_t) noexcept {
  return static_cast<bool>(lhs);
}

template <typename T, typename Deleter>
bool operator!=(std::nullptr_t, const UniquePtr<T, Deleter>& rhs) noexcept {
  return static_cast<bool>(rhs);
}

} // for: namepspace tmn;
