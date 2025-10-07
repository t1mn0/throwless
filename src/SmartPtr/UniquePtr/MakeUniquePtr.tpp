#ifndef TMN_THROWLESS_UNIQUE_PTR_HPP
#error "Include UniquePtr.hpp instead of MakeUniquePtr.tpp"
#endif

#include "../../../include/SmartPtr/UniquePtr.hpp"
#include "../../../include/Result/Result.hpp"

namespace tmn {

template <typename T, typename... Args>
UniquePtr<T> make_unique_ptr(Args&&... args) {
  return UniquePtr<T>(new T(std::forward<Args>(args)...));
}

template <typename T>
UniquePtr<T[]> make_unique_array_ptr(std::size_t size) {
  if (size == 0){
    throw err::InvalidArgErr("Array to be created must have a non-zero size");
  }

  T* ptr = nullptr;

  try {
    ptr = new T[size];
  }
  catch(const std::bad_alloc& e){
    throw err::BadAllocErr();
  }

  return UniquePtr<T[]>(ptr);
}

}
