#ifndef TMN_THROWLESS_UNIQUE_PTR_HPP
#define TMN_THROWLESS_UNIQUE_PTR_HPP

#include "../../Option/Option.hpp"

#include <memory> // for: std::default_delete;

namespace tmn {

// Some abstraction of ownership (primitive simplified level, not Rust):
// UniquePtr wrapper owns the resource if the resource_ptr field is not equal to nullptr.
// Ownership implies the obligation to release physical resources:
// proper destruction of the object and deallocation of memory.
// The second template parameter, Deleter, helps with this;

// 1. General realization:
template <typename T, typename Deleter = std::default_delete<T>>
class UniquePtr {
private: //* fields:
  T* resource_ptr = nullptr;
  [[no_unique_address]] Deleter deleter;

private: // friends:
  template <typename U, typename E>
  friend class UniquePtr;

  friend void swap(UniquePtr<T>& first, UniquePtr<T>& second) noexcept(noexcept(first.swap(second))) {
    first.swap(second);
  }

public: //* methods:
  //*   <--- constructors, (~)ro5, destructor ... --->
  UniquePtr() noexcept;
  explicit UniquePtr(T* resource_ptr) noexcept;

  template <typename U>
  explicit UniquePtr(U* resource_ptr) noexcept;

  // Explicitly prohibiting the reproduction of access to the UniquePtr resource:
  UniquePtr(const UniquePtr& oth) = delete;
  UniquePtr& operator=(const UniquePtr& oth) = delete;

  // But we allow the movement of the UniquePtr resource between owners:
  UniquePtr(UniquePtr<T, Deleter>&& oth) noexcept;
  UniquePtr& operator=(UniquePtr<T, Deleter>&& oth) noexcept;

  ~UniquePtr();

  // Modifiers:
  // Return the Option wrapper over T* if the UniquePtr contains a resource
  // and releases the UniquePtr: `pointer resource_ptr = nullptr`;
  // After this action, the user controls the release of memory over the resource,
  // so the return value should not be skipped!
  [[nodiscard]] Option<T*> try_get_and_free() noexcept;

  // A version of the `try_get_and_free` method with throwing an exception
  // in case of missing resource:
  [[nodiscard]] T* get_and_free();

  // Establishes ownership of a pointer to a resource behind the current UniquePtr object.
  // In this case, the UniquePtr object must be free before calling the method:
  // otherwise, nothing will happen and the method will return `false`.
  // If ownership of the resource has been established, the method returns `true`:
  bool set_resource(T* new_resource) noexcept;

  // Returns pointers to the wrapped resource;
  // These methods may violate the basic idea of using unique pointers
  // and a small prototype of the concept of "ownership", which I adhere to here.
  // It is not recommended to use, as it is not idiomatic:
  Option<T*> try_get() noexcept;
  T* get();

  // Observers:
  bool has_resource() const noexcept;
  explicit operator bool() const noexcept;
  bool operator==(std::nullptr_t) const noexcept;
  bool operator!=(std::nullptr_t) const noexcept;

  Option<T> try_get_val() const noexcept;

  // Returned object is a copy of the resource:
  T get_val() const;

  T operator*() const;

private: //* methods:
  void swap(UniquePtr<T, Deleter>& oth) noexcept;

}; // class UniquePtr<T, Deleter>;

// 2. Specialization UniquePtr<T[]>:
template <typename T>
class UniquePtr<T[], std::default_delete<T[]>> {
private: //* fields :
  T* array_ptr;
  [[no_unique_address]] std::default_delete<T[]> deleter;

public: //* methods:
  //*   <--- constructors, (~)ro5, destructor ... --->
  UniquePtr() noexcept;
  explicit UniquePtr(T* array_ptr) noexcept;

  UniquePtr(const UniquePtr& oth) = delete;
  UniquePtr& operator=(const UniquePtr& oth) = delete;

  UniquePtr(UniquePtr&& oth) noexcept;
  UniquePtr& operator=(UniquePtr&& oth) noexcept;

  ~UniquePtr();

  // Modifiers:
  [[nodiscard]] Option<T*> try_get_and_free() noexcept;
  [[nodiscard]] T* get_and_free();

  bool set_resource() noexcept;

  bool has_resource() const noexcept;
  explicit operator bool() const noexcept;
  bool operator==(std::nullptr_t) const noexcept;
  bool operator!=(std::nullptr_t) const noexcept;

  // Observers:
  Option<T> try_get_val_at(std::size_t index) const noexcept;
  T get_val_at(std::size_t index) const;

  Option<T> operator[](std::size_t index) const noexcept;

}; // class UniquePtr<T[], std::default_delete<T[]>>;

} // for: namespace tmn;

#include "UniquePtr.tpp" // for: UniquePtr definition;

#endif // TMN_THROWLESS_UNIQUE_PTR_HPP
