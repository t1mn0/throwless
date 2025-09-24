#include <gtest/gtest.h>

#include "../../SmartPtr/UniquePtr/UniquePtr.hpp"
#include "../Utils.hpp"

class UniquePtrFixture : public ::testing::Test {
protected:
  void SetUp() override {
    tmn::test_utils::UniqueTestObject::reset_counts();
    tmn::test_utils::UniqueTestDeleter::reset();
  }

  void TearDown() override {
    tmn::test_utils::UniqueTestObject::reset_counts();
    tmn::test_utils::UniqueTestDeleter::reset();
  }
};

TEST_F(UniquePtrFixture, DefaultConstructor) {
  tmn::UniquePtr<tmn::test_utils::UniqueTestObject> ptr;

  EXPECT_FALSE(ptr.has_resource());
  EXPECT_FALSE(static_cast<bool>(ptr));
  EXPECT_EQ(ptr, nullptr);
}

TEST_F(UniquePtrFixture, ConstructorWithPointer) {
  auto* raw_ptr = new tmn::test_utils::UniqueTestObject(42);
  tmn::UniquePtr<tmn::test_utils::UniqueTestObject> ptr(raw_ptr);

  EXPECT_TRUE(ptr.has_resource());
  EXPECT_EQ(ptr.get()->value, 42);
}

TEST_F(UniquePtrFixture, MoveConstructor) {
  auto* raw_ptr = new tmn::test_utils::UniqueTestObject(100);
  tmn::UniquePtr<tmn::test_utils::UniqueTestObject> ptr1(raw_ptr);
  tmn::UniquePtr<tmn::test_utils::UniqueTestObject> ptr2(std::move(ptr1));

  EXPECT_FALSE(ptr1.has_resource());
  EXPECT_TRUE(ptr2.has_resource());
  EXPECT_EQ(ptr2.get()->value, 100);
}

TEST_F(UniquePtrFixture, MoveAssignment) {
  auto* raw_ptr1 = new tmn::test_utils::UniqueTestObject(100);
  auto* raw_ptr2 = new tmn::test_utils::UniqueTestObject(200);

  tmn::UniquePtr<tmn::test_utils::UniqueTestObject> ptr1(raw_ptr1);
  tmn::UniquePtr<tmn::test_utils::UniqueTestObject> ptr2(raw_ptr2);

  ptr2 = std::move(ptr1);

  EXPECT_FALSE(ptr1.has_resource());
  EXPECT_TRUE(ptr2.has_resource());
  EXPECT_EQ(ptr2.get()->value, 100);
}

TEST_F(UniquePtrFixture, DestructorReleasesResource) {
  {
    auto* raw_ptr = new tmn::test_utils::UniqueTestObject(42);
    tmn::UniquePtr<tmn::test_utils::UniqueTestObject> ptr(raw_ptr);
    EXPECT_EQ(tmn::test_utils::UniqueTestObject::destructor_count, 0);
  }

  EXPECT_EQ(tmn::test_utils::UniqueTestObject::destructor_count, 1);
}

TEST_F(UniquePtrFixture, TryGetAndFree) {
  auto* raw_ptr = new tmn::test_utils::UniqueTestObject(42);
  tmn::UniquePtr<tmn::test_utils::UniqueTestObject> ptr(raw_ptr);

  auto result = ptr.try_get_and_free();
  EXPECT_TRUE(result.has_value());
  EXPECT_EQ(result.value()->value, 42);
  EXPECT_FALSE(ptr.has_resource());

  delete result.value();
}

TEST_F(UniquePtrFixture, GetAndFree) {
  auto* raw_ptr = new tmn::test_utils::UniqueTestObject(42);
  tmn::UniquePtr<tmn::test_utils::UniqueTestObject> ptr(raw_ptr);

  auto* freed_ptr = ptr.get_and_free();
  EXPECT_EQ(freed_ptr->value, 42);
  EXPECT_FALSE(ptr.has_resource());

  delete freed_ptr;
}

TEST_F(UniquePtrFixture, SetResource) {
  tmn::UniquePtr<tmn::test_utils::UniqueTestObject> ptr;
  auto* new_raw_ptr = new tmn::test_utils::UniqueTestObject(99);

  EXPECT_TRUE(ptr.set_resource(new_raw_ptr));
  EXPECT_TRUE(ptr.has_resource());
  EXPECT_EQ(ptr.get()->value, 99);
}

TEST_F(UniquePtrFixture, SetResourceFailsWhenNotEmpty) {
  auto* raw_ptr = new tmn::test_utils::UniqueTestObject(42);
  tmn::UniquePtr<tmn::test_utils::UniqueTestObject> ptr(raw_ptr);
  auto* new_raw_ptr = new tmn::test_utils::UniqueTestObject(99);

  EXPECT_FALSE(ptr.set_resource(new_raw_ptr));
  EXPECT_EQ(ptr.get()->value, 42);

  delete new_raw_ptr;
}

TEST_F(UniquePtrFixture, Reset) {
  auto* raw_ptr = new tmn::test_utils::UniqueTestObject(42);
  tmn::UniquePtr<tmn::test_utils::UniqueTestObject> ptr(raw_ptr);

  ptr.reset();
  EXPECT_FALSE(ptr.has_resource());
  EXPECT_EQ(tmn::test_utils::UniqueTestObject::destructor_count, 1);
}

TEST_F(UniquePtrFixture, ResetWithNewResource) {
  auto* raw_ptr1 = new tmn::test_utils::UniqueTestObject(42);
  auto* raw_ptr2 = new tmn::test_utils::UniqueTestObject(99);

  tmn::UniquePtr<tmn::test_utils::UniqueTestObject> ptr(raw_ptr1);
  ptr.reset(raw_ptr2);

  EXPECT_TRUE(ptr.has_resource());
  EXPECT_EQ(ptr.get()->value, 99);
  EXPECT_EQ(tmn::test_utils::UniqueTestObject::destructor_count, 1);
}

TEST_F(UniquePtrFixture, TryGet) {
  auto* raw_ptr = new tmn::test_utils::UniqueTestObject(42);
  tmn::UniquePtr<tmn::test_utils::UniqueTestObject> ptr(raw_ptr);

  auto result = ptr.try_get();
  EXPECT_TRUE(result.has_value());
  EXPECT_EQ(result.value()->value, 42);
  EXPECT_TRUE(ptr.has_resource());
}

TEST_F(UniquePtrFixture, Get) {
  auto* raw_ptr = new tmn::test_utils::UniqueTestObject(42);
  tmn::UniquePtr<tmn::test_utils::UniqueTestObject> ptr(raw_ptr);

  auto* gotten_ptr = ptr.get();
  EXPECT_EQ(gotten_ptr->value, 42);
  EXPECT_TRUE(ptr.has_resource());
}

TEST_F(UniquePtrFixture, DereferenceOperator) {
  auto* raw_ptr = new tmn::test_utils::UniqueTestObject(42);
  tmn::UniquePtr<tmn::test_utils::UniqueTestObject> ptr(raw_ptr);

  EXPECT_EQ((*ptr).value, 42);
  (*ptr).value = 100;
  EXPECT_EQ(ptr.get()->value, 100);
}

TEST_F(UniquePtrFixture, BoolConversion) {
  tmn::UniquePtr<tmn::test_utils::UniqueTestObject> empty_ptr;
  auto* raw_ptr = new tmn::test_utils::UniqueTestObject(42);
  tmn::UniquePtr<tmn::test_utils::UniqueTestObject> valid_ptr(raw_ptr);

  EXPECT_FALSE(empty_ptr);
  EXPECT_TRUE(valid_ptr);
}

TEST_F(UniquePtrFixture, Swap) {
  auto* raw_ptr1 = new tmn::test_utils::UniqueTestObject(42);
  auto* raw_ptr2 = new tmn::test_utils::UniqueTestObject(99);

  tmn::UniquePtr<tmn::test_utils::UniqueTestObject> ptr1(raw_ptr1);
  tmn::UniquePtr<tmn::test_utils::UniqueTestObject> ptr2(raw_ptr2);

  swap(ptr1, ptr2);

  EXPECT_EQ(ptr1.get()->value, 99);
  EXPECT_EQ(ptr2.get()->value, 42);
}

TEST_F(UniquePtrFixture, CustomDeleter) {
  auto* raw_ptr = new tmn::test_utils::UniqueTestObject(42);
  tmn::UniquePtr<tmn::test_utils::UniqueTestObject, tmn::test_utils::UniqueTestDeleter> ptr(raw_ptr);

  EXPECT_EQ(tmn::test_utils::UniqueTestDeleter::delete_count, 0);
  ptr.reset();
  EXPECT_EQ(tmn::test_utils::UniqueTestDeleter::delete_count, 1);
  EXPECT_EQ(tmn::test_utils::UniqueTestDeleter::last_deleted_value, 42);
}

TEST_F(UniquePtrFixture, TemplateConstructor) {
  struct Base { virtual ~Base() = default; int base_val = 10; };
  struct Derived : Base { int derived_val = 20; };

  auto* derived_ptr = new Derived;
  tmn::UniquePtr<Base> ptr(derived_ptr);

  EXPECT_TRUE(ptr.has_resource());
  EXPECT_EQ(ptr.get()->base_val, 10);
}
