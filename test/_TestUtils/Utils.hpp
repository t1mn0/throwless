#ifndef TMN_THROWLESS_TEST_UTILS_HPP
#define TMN_THROWLESS_TEST_UTILS_HPP

#include <cstddef>

namespace tmn::test_utils {

struct ObjectCounters {
  static inline int constructor_count = 0;
  static inline int destructor_count = 0;
  static inline int copy_count = 0;
  static inline int move_count = 0;

  static void reset_counts() {
    constructor_count = destructor_count = copy_count = move_count = 0;
  }
};

struct TestObject : ObjectCounters {
  int value;
  int id;

  TestObject(int val = 0) : value(val), id(++constructor_count) {}

  TestObject(const TestObject& oth) : value(oth.value), id(++constructor_count) {
    ++copy_count;
  }

  TestObject(TestObject&& oth) noexcept : value(oth.value), id(oth.id) {
    ++move_count;
    oth.value = -1;
  }

  TestObject& operator=(const TestObject& oth) {
    if (this != &oth) {
      value = oth.value;
      id = ++constructor_count;
      ++copy_count;
    }
    return *this;
  }

  TestObject& operator=(TestObject&& oth) noexcept {
    if (this != &oth) {
      value = oth.value;
      id = oth.id;
      ++move_count;
      oth.value = -1;
    }
    return *this;
  }

  ~TestObject() { ++destructor_count; }
};

struct SimpleTestObject {
  static inline int constructor_count = 0;
  static inline int destructor_count = 0;

  int value;
  int id;

  SimpleTestObject(int val = 0) : value(val), id(++constructor_count) {}
  ~SimpleTestObject() { ++destructor_count; }

  static void reset_counts() {
    constructor_count = destructor_count = 0;
  }
};

template<typename T>
struct BasicDeleter {
  static inline int delete_count = 0;
  static inline int last_deleted_value = 0;

  void operator()(T* ptr) {
    if (ptr) {
      last_deleted_value = ptr->value;
      delete ptr;
      ++delete_count;
    }
  }

  static void reset() {
    delete_count = 0;
    last_deleted_value = 0;
  }
};

template<typename T>
struct ArrayDeleter {
  static inline int delete_count = 0;
  static inline int last_deleted_size = 0;

  void operator()(T* ptr, size_t size) {
    if (ptr) {
      last_deleted_size = static_cast<int>(size);
      for (size_t i = 0; i < size; ++i) {
          ptr[i].value = -999;
      }
      delete[] ptr;
      ++delete_count;
    }
  }

  static void reset() {
    delete_count = 0;
    last_deleted_size = 0;
  }
};

using UniqueTestObject = TestObject;
using UniqueTestDeleter = BasicDeleter<TestObject>;

using SharedTestObject = TestObject;
using SharedTestDeleter = BasicDeleter<TestObject>;
using SharedTestArrayDeleter = ArrayDeleter<TestObject>;

using WeakTestObject = SimpleTestObject;
using WeakTestDeleter = BasicDeleter<SimpleTestObject>;

} // namespace tmn::test_utils;

#endif // TMN_THROWLESS_TEST_UTILS_HPP;
