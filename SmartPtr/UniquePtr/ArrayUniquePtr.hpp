#ifndef TMN_THROWLESS_UNIQUE_PTR_HPP
#error "Include UniquePtr.hpp instead of ArrayUniquePtr.hpp"
#endif

#include "UniquePtr.hpp"
#include "../../Result/Result.hpp"
#include "../../Error/Error.hpp"

namespace tmn {

template <typename T>
class UniquePtr<T[], std::default_delete<T[]>> {
private: //* fields :
  T* array_ptr = nullptr;
  [[no_unique_address]] std::default_delete<T[]> deleter;

public: //* methods:
  //*   <--- constructors, (~)ro5, destructor ... --->
  UniquePtr() noexcept;
  explicit UniquePtr(T* array_ptr) noexcept;

  UniquePtr(const UniquePtr&) = delete;
  UniquePtr& operator=(const UniquePtr&) = delete;

  UniquePtr(UniquePtr&& oth) noexcept;
  UniquePtr& operator=(UniquePtr&& oth) noexcept;

  ~UniquePtr();

  // Modifiers:
  [[nodiscard]] T* get_and_free();
  [[nodiscard]] Option<T*> try_get_and_free() noexcept;

  bool set_resource(T* new_resource) noexcept;
  void reset(T* new_resource = nullptr) noexcept;

  T* get();
  Option<T*> try_get() noexcept;

  bool has_resource() const noexcept;
  explicit operator bool() const noexcept;
  bool operator==(std::nullptr_t) const noexcept;
  bool operator!=(std::nullptr_t) const noexcept;

  // Observers:
  Result<T, err::EmptyArrErr> at(std::size_t index) const noexcept;

  T& operator[](std::size_t index) &;
  const T& operator[](std::size_t index) const&;

private:
  void swap(UniquePtr& oth) noexcept;

public: //* friends:
  friend void swap(UniquePtr<T[]>& first, UniquePtr<T[]>& second) noexcept(noexcept(first.swap(second))) {
    first.swap(second);
  }

}; // class UniquePtr<T[], std::default_delete<T[]>>;

} // for: namespace tmn;

#include "ArrayUniquePtr.tpp" // for: UniquePtr<T[]> definition;
