#ifndef TMN_THROWLESS_TEST_UTILS_HPP
#define TMN_THROWLESS_TEST_UTILS_HPP

namespace tmn::test_utils {

struct UniqueTestObject {
  static int constructor_count;
  static int destructor_count;
  static int copy_count;
  static int move_count;

  int value;

  UniqueTestObject(int val = 0) : value(val) { constructor_count++; }
  ~UniqueTestObject() { destructor_count++; }

  UniqueTestObject(const UniqueTestObject& oth) : value(oth.value) { copy_count++; }
  UniqueTestObject(UniqueTestObject&& oth) noexcept : value(oth.value) {
    move_count++;
    oth.value = -1;
  }

  UniqueTestObject& operator=(const UniqueTestObject& oth) {
    value = oth.value;
    copy_count++;
    return *this;
  }

  UniqueTestObject& operator=(UniqueTestObject&& oth) noexcept {
    value = oth.value;
    oth.value = -1;
    move_count++;
    return *this;
  }

  static void reset_counts() {
    constructor_count = destructor_count = copy_count = move_count = 0;
  }
};

struct UniqueTestDeleter {
  static int delete_count;
  static int last_deleted_value;

  void operator()(UniqueTestObject* ptr) {
    if (ptr) {
      last_deleted_value = ptr->value;
      delete_count++;
      delete ptr;
    }
  }

  static void reset() {
    delete_count = 0;
    last_deleted_value = 0;
  }
};

struct SharedTestObject {
  static inline int constructor_count = 0;
  static inline int destructor_count = 0;
  static inline int copy_count = 0;
  static inline int move_count = 0;

  int value;
  int id;

  SharedTestObject(int val = 0) : value(val), id(++constructor_count) {}

  SharedTestObject(const SharedTestObject& oth)
    : value(oth.value), id(++constructor_count)
  {
    ++copy_count;
  }

  SharedTestObject(SharedTestObject&& oth) noexcept
    : value(oth.value), id(oth.id)
  {
    ++move_count;
    oth.value = -1;
  }

  ~SharedTestObject() {
    ++destructor_count;
  }

  static void reset_counts() {
    constructor_count = 0;
    destructor_count = 0;
    copy_count = 0;
    move_count = 0;
  }
};

struct SharedTestDeleter {
  static inline int delete_count = 0;
  static inline int last_deleted_value = 0;

  void operator()(SharedTestObject* ptr) {
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

} // namespace tmn::test_utils;

#endif // TMN_THROWLESS_TEST_UTILS_HPP
