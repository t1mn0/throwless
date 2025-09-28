#include <gtest/gtest.h>

#include "../../SmartPtr/SharedPtr/SharedPtr.hpp"
#include "../Utils.hpp"

class SharedPtrArrayTest : public ::testing::Test {
protected:
  void SetUp() override {
    tmn::test_utils::SharedTestObject::reset_counts();
    tmn::test_utils::SharedTestArrayDeleter::reset();
  }

  void TearDown() override {
    tmn::test_utils::SharedTestObject::reset_counts();
    tmn::test_utils::SharedTestArrayDeleter::reset();
  }
};

TEST_F(SharedPtrArrayTest, DefaultConstructor) {
  tmn::SharedPtr<tmn::test_utils::SharedTestObject[]> ptr;

  EXPECT_FALSE(ptr);
  EXPECT_EQ(ptr.get(), nullptr);
  EXPECT_EQ(ptr.use_count(), 0);
  EXPECT_EQ(ptr.size(), 0);
  EXPECT_TRUE(ptr.empty());
}

TEST_F(SharedPtrArrayTest, ConstructorWithArray) {
  tmn::test_utils::SharedTestObject* arr = new tmn::test_utils::SharedTestObject[3]{10, 20, 30};
  tmn::SharedPtr<tmn::test_utils::SharedTestObject[]> ptr(arr, 3);

  EXPECT_TRUE(ptr);
  EXPECT_EQ(ptr.get(), arr);
  EXPECT_EQ(ptr.use_count(), 1);
  EXPECT_EQ(ptr.size(), 3);
  EXPECT_FALSE(ptr.empty());
  EXPECT_EQ(ptr[0].value, 10);
  EXPECT_EQ(ptr[2].value, 30);
}

TEST_F(SharedPtrArrayTest, ConstructorWithCustomDeleter) {
  tmn::test_utils::SharedTestObject* arr = new tmn::test_utils::SharedTestObject[2]{40, 50};

  {
    tmn::SharedPtr<tmn::test_utils::SharedTestObject[]> ptr(arr, 2, tmn::test_utils::SharedTestArrayDeleter());
    EXPECT_TRUE(ptr);
    EXPECT_EQ(ptr.use_count(), 1);
  }

  EXPECT_EQ(tmn::test_utils::SharedTestArrayDeleter::delete_count, 1);
  EXPECT_EQ(tmn::test_utils::SharedTestArrayDeleter::last_deleted_size, 2);
}

TEST_F(SharedPtrArrayTest, CopyConstructor) {
  tmn::SharedPtr<tmn::test_utils::SharedTestObject[]> ptr1(new tmn::test_utils::SharedTestObject[2]{100, 200}, 2);
  tmn::SharedPtr<tmn::test_utils::SharedTestObject[]> ptr2(ptr1);

  EXPECT_EQ(ptr1.use_count(), 2);
  EXPECT_EQ(ptr2.use_count(), 2);
  EXPECT_EQ(ptr1.get(), ptr2.get());
  EXPECT_EQ(ptr1.size(), ptr2.size());
  EXPECT_EQ(ptr1[0].value, 100);
  EXPECT_EQ(ptr2[1].value, 200);
}

TEST_F(SharedPtrArrayTest, CopyAssignment) {
  tmn::SharedPtr<tmn::test_utils::SharedTestObject[]> ptr1(new tmn::test_utils::SharedTestObject[3]{300, 400, 500}, 3);
  tmn::SharedPtr<tmn::test_utils::SharedTestObject[]> ptr2;

  ptr2 = ptr1;

  EXPECT_EQ(ptr1.use_count(), 2);
  EXPECT_EQ(ptr2.use_count(), 2);
  EXPECT_EQ(ptr1.get(), ptr2.get());
  EXPECT_EQ(ptr1[2].value, 500);
  EXPECT_EQ(ptr2[0].value, 300);
}

TEST_F(SharedPtrArrayTest, MoveConstructor) {
  tmn::SharedPtr<tmn::test_utils::SharedTestObject[]> ptr1(new tmn::test_utils::SharedTestObject[2]{700, 800}, 2);
  tmn::test_utils::SharedTestObject* original_ptr = ptr1.get();
  size_t original_size = ptr1.size();

  tmn::SharedPtr<tmn::test_utils::SharedTestObject[]> ptr2(std::move(ptr1));

  EXPECT_FALSE(ptr1);
  EXPECT_EQ(ptr1.use_count(), 0);
  EXPECT_EQ(ptr1.size(), 0);
  EXPECT_TRUE(ptr2);
  EXPECT_EQ(ptr2.use_count(), 1);
  EXPECT_EQ(ptr2.get(), original_ptr);
  EXPECT_EQ(ptr2.size(), original_size);
  EXPECT_EQ(ptr2[1].value, 800);
}

TEST_F(SharedPtrArrayTest, MoveAssignment) {
  tmn::SharedPtr<tmn::test_utils::SharedTestObject[]> ptr1(new tmn::test_utils::SharedTestObject[2]{900, 1000}, 2);
  tmn::SharedPtr<tmn::test_utils::SharedTestObject[]> ptr2(new tmn::test_utils::SharedTestObject[1]{1100}, 1);

  tmn::test_utils::SharedTestObject* original_ptr = ptr1.get();
  ptr2 = std::move(ptr1);

  EXPECT_FALSE(ptr1);
  EXPECT_TRUE(ptr2);
  EXPECT_EQ(ptr2.get(), original_ptr);
  EXPECT_EQ(ptr2.size(), 2);
  EXPECT_EQ(ptr2[0].value, 900);
}

TEST_F(SharedPtrArrayTest, ArraySubscriptOperator) {
  tmn::SharedPtr<tmn::test_utils::SharedTestObject[]> ptr(new tmn::test_utils::SharedTestObject[3]{1, 2, 3}, 3);

  EXPECT_EQ(ptr[0].value, 1);
  EXPECT_EQ(ptr[1].value, 2);
  EXPECT_EQ(ptr[2].value, 3);

  ptr[1].value = 1245;
  EXPECT_EQ(ptr[1].value, 1245);
}

TEST_F(SharedPtrArrayTest, AtMethodSuccess) {
  tmn::SharedPtr<tmn::test_utils::SharedTestObject[]> ptr(new tmn::test_utils::SharedTestObject[2]{10, 20}, 2);

  auto result1 = ptr.at(0);
  EXPECT_TRUE(result1.is_ok());
  EXPECT_EQ(result1.unwrap_value().value, 10);

  auto result2 = ptr.at(1);
  EXPECT_TRUE(result2.is_ok());
  EXPECT_EQ(result2.unwrap_value().value, 20);
}

TEST_F(SharedPtrArrayTest, AtMethodErrorCases) {
  tmn::SharedPtr<tmn::test_utils::SharedTestObject[]> ptr(new tmn::test_utils::SharedTestObject[2]{30, 40}, 2);

  auto result1 = ptr.at(2);
  EXPECT_TRUE(result1.is_err());

  tmn::SharedPtr<tmn::test_utils::SharedTestObject[]> empty_ptr;
  auto result2 = empty_ptr.at(0);
  EXPECT_TRUE(result2.is_err());
}

TEST_F(SharedPtrArrayTest, GetAndTryGet) {
  tmn::SharedPtr<tmn::test_utils::SharedTestObject[]> ptr(new tmn::test_utils::SharedTestObject[2]{50, 60}, 2);

  EXPECT_NE(ptr.get(), nullptr);
  auto try_result = ptr.try_get();
  EXPECT_TRUE(try_result.has_value());
  EXPECT_EQ(try_result.value(), ptr.get());

  tmn::SharedPtr<tmn::test_utils::SharedTestObject[]> empty_ptr;
  EXPECT_EQ(empty_ptr.get(), nullptr);
  EXPECT_FALSE(empty_ptr.try_get().has_value());
}

TEST_F(SharedPtrArrayTest, Observers) {
  tmn::SharedPtr<tmn::test_utils::SharedTestObject[]> ptr(new tmn::test_utils::SharedTestObject[3]{100, 200, 300}, 3);

  EXPECT_TRUE(ptr.has_resource());
  EXPECT_TRUE(static_cast<bool>(ptr));
  EXPECT_EQ(ptr.use_count(), 1);
  EXPECT_TRUE(ptr.is_unique());
  EXPECT_EQ(ptr.size(), 3);
  EXPECT_FALSE(ptr.empty());
}

TEST_F(SharedPtrArrayTest, UseCountWithMultipleReferences) {
  tmn::SharedPtr<tmn::test_utils::SharedTestObject[]> ptr1(new tmn::test_utils::SharedTestObject[2]{400, 500}, 2);
  EXPECT_EQ(ptr1.use_count(), 1);
  EXPECT_TRUE(ptr1.is_unique());

  tmn::SharedPtr<tmn::test_utils::SharedTestObject[]> ptr2 = ptr1;
  EXPECT_EQ(ptr1.use_count(), 2);
  EXPECT_EQ(ptr2.use_count(), 2);
  EXPECT_FALSE(ptr1.is_unique());
  EXPECT_FALSE(ptr2.is_unique());

  tmn::SharedPtr<tmn::test_utils::SharedTestObject[]> ptr3 = ptr2;
  EXPECT_EQ(ptr1.use_count(), 3);
  EXPECT_EQ(ptr2.use_count(), 3);
  EXPECT_EQ(ptr3.use_count(), 3);
}

TEST_F(SharedPtrArrayTest, Iterators) {
  tmn::SharedPtr<tmn::test_utils::SharedTestObject[]> ptr(new tmn::test_utils::SharedTestObject[4]{1, 2, 3, 4}, 4);

  int sum = 0;
  for (tmn::test_utils::SharedTestObject* it = ptr.begin(); it != ptr.end(); ++it) {
    sum += it->value;
  }
  EXPECT_EQ(sum, 10);

  for (tmn::test_utils::SharedTestObject* it = ptr.begin(); it != ptr.end(); ++it) {
    it->value *= 2;
  }
  EXPECT_EQ(ptr[0].value, 2);
  EXPECT_EQ(ptr[3].value, 8);
}

TEST_F(SharedPtrArrayTest, EmptyArrayIterators) {
  tmn::SharedPtr<tmn::test_utils::SharedTestObject[]> empty_ptr;
  EXPECT_EQ(empty_ptr.begin(), nullptr);
  EXPECT_EQ(empty_ptr.end(), nullptr);
}

TEST_F(SharedPtrArrayTest, ResetWithNewArray) {
  tmn::SharedPtr<tmn::test_utils::SharedTestObject[]> ptr(new tmn::test_utils::SharedTestObject[2]{1, 2}, 2);
  ptr.reset(new tmn::test_utils::SharedTestObject[3]{10, 20, 30}, 3);

  EXPECT_TRUE(ptr);
  EXPECT_EQ(ptr.use_count(), 1);
  EXPECT_EQ(ptr.size(), 3);
  EXPECT_EQ(ptr[2].value, 30);
}

TEST_F(SharedPtrArrayTest, ResetWithDeleter) {
  tmn::SharedPtr<tmn::test_utils::SharedTestObject[]> ptr;
  ptr.reset(new tmn::test_utils::SharedTestObject[2]{40, 50}, 2, tmn::test_utils::SharedTestArrayDeleter());

  EXPECT_TRUE(ptr);
  EXPECT_EQ(ptr.use_count(), 1);

  ptr.reset();

  EXPECT_EQ(tmn::test_utils::SharedTestArrayDeleter::delete_count, 1);
  EXPECT_EQ(tmn::test_utils::SharedTestArrayDeleter::last_deleted_size, 2);
}

TEST_F(SharedPtrArrayTest, ResetToNullptr) {
  tmn::SharedPtr<tmn::test_utils::SharedTestObject[]> ptr(new tmn::test_utils::SharedTestObject[1]{60}, 1);
  ptr.reset();

  EXPECT_FALSE(ptr);
  EXPECT_EQ(ptr.use_count(), 0);
  EXPECT_EQ(ptr.size(), 0);
}

TEST_F(SharedPtrArrayTest, EqualityComparison) {
  tmn::SharedPtr<int[]> ptr1(new int[2]{1, 2}, 2);
  tmn::SharedPtr<int[]> ptr2 = ptr1;
  tmn::SharedPtr<int[]> ptr3(new int[2]{1, 2}, 2);

  EXPECT_EQ(ptr1, ptr2);
  EXPECT_NE(ptr1, ptr3);
}

TEST_F(SharedPtrArrayTest, NullptrComparison) {
  tmn::SharedPtr<int[]> ptr1(new int[1]{1}, 1);
  tmn::SharedPtr<int[]> ptr2;

  EXPECT_NE(ptr1, nullptr);
  EXPECT_EQ(ptr2, nullptr);
  EXPECT_EQ(nullptr, ptr2);
  EXPECT_NE(nullptr, ptr1);
}

TEST_F(SharedPtrArrayTest, Swap) {
  tmn::SharedPtr<tmn::test_utils::SharedTestObject[]> ptr1(new tmn::test_utils::SharedTestObject[2]{100, 200}, 2);
  tmn::SharedPtr<tmn::test_utils::SharedTestObject[]> ptr2(new tmn::test_utils::SharedTestObject[1]{300}, 1);

  tmn::test_utils::SharedTestObject* ptr1_original = ptr1.get();
  tmn::test_utils::SharedTestObject* ptr2_original = ptr2.get();
  size_t ptr1_size = ptr1.size();
  size_t ptr2_size = ptr2.size();

  swap(ptr1, ptr2);

  EXPECT_EQ(ptr1.get(), ptr2_original);
  EXPECT_EQ(ptr2.get(), ptr1_original);
  EXPECT_EQ(ptr1.size(), ptr2_size);
  EXPECT_EQ(ptr2.size(), ptr1_size);
  EXPECT_EQ(ptr1[0].value, 300);
  EXPECT_EQ(ptr2[1].value, 200);
}

TEST_F(SharedPtrArrayTest, DestructorCallsDeleter) {
  {
    tmn::SharedPtr<tmn::test_utils::SharedTestObject[]> ptr(new tmn::test_utils::SharedTestObject[2]{1, 2}, 2, tmn::test_utils::SharedTestArrayDeleter());
  }

  EXPECT_EQ(tmn::test_utils::SharedTestArrayDeleter::delete_count, 1);
  EXPECT_EQ(tmn::test_utils::SharedTestArrayDeleter::last_deleted_size, 2);
}
