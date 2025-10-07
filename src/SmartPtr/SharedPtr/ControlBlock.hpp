#ifndef TMN_THROWLESS_SHARED_PTR_HPP
#error \
"ControlBlock.hpp is a file with hidden implementation details\
of the SmartPtr mechanism and is not intended for use"
#endif

#ifndef TMN_THROWLESS_SHARED_PTR_CONTROL_BLOCK_HPP
#define TMN_THROWLESS_SHARED_PTR_CONTROL_BLOCK_HPP

#include <atomic> // for: std::atomic;
#include <functional> // for: std::function;

namespace tmn {

// Forward declaration to declare friendship:
template<typename T>
class WeakPtr;

template<typename T>
class SharedPtr;

// ControlBlock shares resource lifetime management between all smart
// pointers, ensuring secure sharing and automatic release when strong references end;
template<typename T>
struct ControlBlock {
private: //* fields:
  std::atomic<size_t> ref_count;
  std::atomic<size_t> weak_count;

  // ControlBlock.object_ptr is the same pointer SharedPtr.resource_ptr;
  // (!) We can optimize the storage of an object created through `make_shared` if we create
  // a polymorphic CB analog that is will be aligned according to type T
  // => this will allow for a single memory allocation for two objects at once (CB, resource)
  // instead of two different ones.
  // However, I haven't figured out how to avoid the `aligned_*` that clang considers
  // "deprecated" in use
  // (https://stackoverflow.com/questions/71828288/why-is-stdaligned-storage-to-be-deprecated-in-c23-and-what-to-use-instead)
  // Therefore, in my implementation there is no such optimization for sharedptrs created through make_shared:
  T* object_ptr;
  std::function<void(T*)> deleter;

private: //* friends:
  template<typename U>
  friend class WeakPtr;

  template<typename U>
  friend class SharedPtr;

public:
  //* constructors:
  ControlBlock(T* ptr);
  ControlBlock(T* ptr, std::function<void(T*)> del);

  //* modifier-methods:
  void increment_strong() noexcept;
  bool decrement_strong() noexcept;
  void increment_weak() noexcept;
  bool decrement_weak() noexcept;
  size_t counter_value() const noexcept;

  bool operator==(const ControlBlock& oth) const noexcept = default;
  bool operator!=(const ControlBlock& oth) const noexcept = default;
};

} // namespace tmn;

#include "ControlBlock.tpp"
#include "ArrayControlBlock.hpp"

#endif // TMN_THROWLESS_SHARED_PTR_CONTROL_BLOCK_HPP
