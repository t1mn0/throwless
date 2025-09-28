#ifndef TMN_THROWLESS_SHARED_PTR_HPP
#error "Include SharedPtr.hpp instead of ArraySharedPtr.hpp"
#endif

#include "SharedPtr.hpp"
#include "ArrayControlBlock.hpp"
#include "../../Result/Result.hpp"
#include "../../Error/Error.hpp"

namespace tmn {

// Forward declaration for friendship:
template<typename T>
class SharedPtr;

template<typename T>
class WeakPtr;

// Specialization for arrays T[]
template<typename T>
class SharedPtr<T[]> {
private: //* fields:
  T* resource_ptr = nullptr;
  ArrayControlBlock<T>* control_block = nullptr;

private: //* methods:
  void cleanup() noexcept;

  template<typename U> requires std::is_convertible_v<U(*)[], T(*)[]>
  void copy_from(const SharedPtr<U[]>& other) noexcept;

  void swap(SharedPtr& other) noexcept;

  friend void swap(SharedPtr<T[]>& first, SharedPtr<T[]>& second)
    noexcept(noexcept(first.swap(second)))
  {
    first.swap(second);
  }

public:
//*   <--- constructors, (~)ro5, destructor (etc) --->
  SharedPtr() noexcept = default;
  explicit SharedPtr(T* ptr, size_t size);

  template<typename Deleter>
  SharedPtr(T* ptr, size_t size, Deleter deleter);

  template<typename U> requires std::is_convertible_v<U(*)[], T(*)[]>
  SharedPtr(const SharedPtr<U[]>& oth) noexcept;

  SharedPtr(const SharedPtr& oth) noexcept;
  SharedPtr(SharedPtr&& oth) noexcept;

  template<typename U> requires std::is_convertible_v<U(*)[], T(*)[]>
  SharedPtr& operator=(const SharedPtr<U[]>& oth) noexcept;

  SharedPtr& operator=(const SharedPtr& oth) noexcept;
  SharedPtr& operator=(SharedPtr&& oth) noexcept;

  ~SharedPtr();

//*   <--- methods --->
//* Observers:
  T* get() const noexcept;
  Option<T*> try_get() const noexcept;

  T& operator[](size_t index) const noexcept;

  Result<T, err::AnyErr> at(size_t index) const;

  bool has_resource() const noexcept;
  explicit operator bool() const noexcept;
  size_t use_count() const noexcept;
  bool is_unique() const noexcept;

  size_t size() const noexcept;
  bool empty() const noexcept;

//* Iterators:
  T* begin() const noexcept;
  T* end() const noexcept;

//* Modifiers:
  void reset(T* new_ptr = nullptr, size_t new_size = 0);

  template<typename Deleter>
  void reset(T* new_ptr, size_t new_size, Deleter deleter);

//*   <--- comparison operations --->
  bool operator==(const SharedPtr& other) const noexcept;
  bool operator!=(const SharedPtr& other) const noexcept;
};

// Global comparison with nullptr
template<typename T>
bool operator==(const SharedPtr<T[]>& lhs, std::nullptr_t) noexcept;

template<typename T>
bool operator==(std::nullptr_t, const SharedPtr<T[]>& rhs) noexcept;

template<typename T>
bool operator!=(const SharedPtr<T[]>& lhs, std::nullptr_t) noexcept;

template<typename T>
bool operator!=(std::nullptr_t, const SharedPtr<T[]>& rhs) noexcept;

} // for: namespace tmn;

#include "ArraySharedPtr.tpp"
