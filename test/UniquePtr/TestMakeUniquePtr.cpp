#include <gtest/gtest.h>

#include "../../include/SmartPtr/UniquePtr.hpp"

class UniquePtrMakerFixture : public ::testing::Test {
protected:
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(UniquePtrMakerFixture, MakeUniquePtrSingleObject) {
  auto ptr = tmn::make_unique_ptr<int>(42);
  ASSERT_NE(ptr, nullptr);
  EXPECT_EQ(*ptr, 42);

  struct TestStruct {
    int a;
    double b;
    TestStruct(int x, double y) : a(x), b(y) {}
  };

  auto ptr2 = tmn::make_unique_ptr<TestStruct>(10, 3.14);
  ASSERT_NE(ptr2, nullptr);
  EXPECT_EQ(ptr2->a, 10);
  EXPECT_DOUBLE_EQ(ptr2->b, 3.14);
}

TEST_F(UniquePtrMakerFixture, MakeUniquePtrArraySuccess) {
  auto result = tmn::make_unique_array_ptr<int>(5);

  ASSERT_NE(result, nullptr);

  for (int i = 0; i < 5; ++i) {
    result[i] = i * 10;
  }

  for (int i = 0; i < 5; ++i) {
    EXPECT_EQ(result[i], i * 10);
  }
}

TEST_F(UniquePtrMakerFixture, MakeUniquePtrArrayZeroSize) {
  ASSERT_THROW(auto t = tmn::make_unique_array_ptr<int>(0), tmn::err::InvalidArgErr);
}

TEST_F(UniquePtrMakerFixture, MakeUniquePtrArrayLargeSize) {
  ASSERT_THROW(auto t = tmn::make_unique_array_ptr<int>(SIZE_MAX / sizeof(int) + 1), tmn::err::BadAllocErr);
}
