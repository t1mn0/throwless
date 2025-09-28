#ifndef TMN_THROWLESS_SHARED_PTR_HPP
#define TMN_THROWLESS_SHARED_PTR_HPP

#include "ControlBlock.hpp"
#include "../../Option/Option.hpp"

namespace tmn {

template<typename T>
class SharedPtr {
private: //* fields:
  T* resource_ptr = nullptr;
  ControlBlock<T>* control_block = nullptr;

private: //* methods:
  // cleanup() and copy_from() methods are most often used in conjunction.
  // This is not the case only in the case of constructing an object without
  // an active ControlBlock by copying;
  // cleanup() -> controlblock_ptr = nullptr && resource_ptr == nullptr;
  void cleanup() noexcept;

  // only with clean objects: after cleanup() or during construction:
  template<typename U> requires std::is_convertible_v<U*, T*>
  void copy_from(const SharedPtr<U>& oth) noexcept;

  // will make the `swap()` private and delegate it to the overload-friend std::swap:
  void swap(SharedPtr& oth) noexcept;

  friend void swap(SharedPtr<T>& first, SharedPtr<T>& second) noexcept(noexcept(first.swap(second))) {
    first.swap(second);
  }

public:
//*   <--- constructors, (~)ro5, destructor (etc) --->
  SharedPtr() noexcept = default;
  explicit SharedPtr(T* ptr);

  template<typename Deleter>
  SharedPtr(T* ptr, Deleter deleter);

  template<typename U> requires std::is_convertible_v<U*, T*>
  SharedPtr(const SharedPtr<U>& oth) noexcept;

  SharedPtr(const SharedPtr& oth);
  SharedPtr(SharedPtr&& oth) noexcept;

  template<typename U> requires std::is_convertible_v<U*, T*>
  SharedPtr& operator=(const SharedPtr<U>& oth) noexcept;

  SharedPtr& operator=(const SharedPtr& oth) noexcept;
  SharedPtr& operator=(SharedPtr&& oth) noexcept;

  ~SharedPtr();

  // Observers:
  T* get() const noexcept;
  Option<T*> try_get() const noexcept;
  T& operator*() const noexcept;
  T* operator->() const noexcept;

  explicit operator bool() const noexcept;
  size_t use_count() const noexcept;
  bool is_unique() const noexcept;

  // Modifiers:
  void reset(T* new_ptr = nullptr);

  template<typename Deleter>
  void reset(T* new_ptr, Deleter deleter);

  // Ptr comparison operations:
  bool operator==(const SharedPtr& oth) const noexcept;
  bool operator!=(const SharedPtr& oth) const noexcept;
  bool operator>(const SharedPtr& oth) const noexcept;
  bool operator<(const SharedPtr& oth) const noexcept;
};

} // namespace tmn;

#include "ArraySharedPtr.hpp"
#include "SharedPtr.tpp"

#endif // TMN_THROWLESS_SHARED_PTR_HPP
