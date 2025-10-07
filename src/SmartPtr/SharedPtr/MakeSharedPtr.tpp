#ifndef TMN_THROWLESS_SHARED_PTR_HPP
#error "Include SharedPtr.hpp instead of MakeSharedPtr.tpp"
#endif

#include "../../../include/SmartPtr/SharedPtr.hpp"
#include "../../../include/Result/Result.hpp"
#include "ControlBlock.hpp"

namespace tmn {

// to see why I don't have an optimization in the form of aligned_storage_t<sizeof(T), alignof(T)>
// for CB for a single memory allocation instead of a double one,
// see the comment in the ControlBlock.hpp file;

template <typename T, typename... Args>
SharedPtr<T> make_shared_ptr(Args&&... args) {
  T* raw_resource_ptr = nullptr;
  ControlBlock<T>* control_block = nullptr;

  try {
    raw_resource_ptr = new T(std::forward<Args>(args)...);
    control_block = new ControlBlock<T>(raw_resource_ptr);
  }
  catch(...){
    delete raw_resource_ptr;
    throw;
  }

  return SharedPtr<T>(raw_resource_ptr, control_block);
}

template<typename T>
Result<SharedPtr<T[]>, err::AnyErr> make_shared_array(size_t size) {
  if (size == 0) {
    return Result<SharedPtr<T[]>, err::InvalidArgErr>::Err(
      err::InvalidArgErr("Array to be created must have a non-zero size")
    );
  }

  T* raw_array = nullptr;
  ControlBlock<T[]>* control_block = nullptr;

  try {
    raw_array = new T[size];

    control_block = new ControlBlock<T[]>(raw_array, size);
  }
  catch (const std::bad_alloc& e) {
    return Result<SharedPtr<T[]>, err::BadAllocErr>::Err(
      err::BadAllocErr()
    );
  }

  assert(raw_array != nullptr);
  assert(control_block != nullptr);

  return Result<SharedPtr<T[]>, err::BadAllocErr>::Ok(std::move(SharedPtr<T[]>(raw_array, size, control_block)));
}

}
