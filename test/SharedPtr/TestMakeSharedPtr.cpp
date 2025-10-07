#include <gtest/gtest.h>

#include "../../include/SmartPtr/SharedPtr.hpp"

class SharedPtrMakerFixture : public ::testing::Test {
protected:
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(SharedPtrMakerFixture, MakeSharedPtrSingleObject) {
  auto ptr = tmn::make_shared_ptr<std::string>("Hello, World!");

  ASSERT_NE(ptr, nullptr);
  EXPECT_EQ(*ptr, "Hello, World!");
  EXPECT_EQ(ptr->size(), 13);
  EXPECT_EQ(ptr.counter_value(), 1);
}

TEST_F(SharedPtrMakerFixture, MakeSharedPtrWithWeakPtr) {
  tmn::WeakPtr<int> weak_ptr;

  {
    auto shared_ptr = tmn::make_shared_ptr<int>(42);
    weak_ptr = shared_ptr;

    EXPECT_FALSE(weak_ptr.is_expired());
    auto locked = weak_ptr.promote();
    ASSERT_NE(locked, nullptr);
    EXPECT_EQ(*locked, 42);
    EXPECT_EQ(shared_ptr.counter_value(), 2);
  }

  EXPECT_TRUE(weak_ptr.is_expired());
  EXPECT_EQ(weak_ptr.promote(), nullptr);
}

TEST_F(SharedPtrMakerFixture, MakeUniquePtrArrayZeroSize) {
  auto t = tmn::make_shared_array<int>(0);
  ASSERT_TRUE(t.is_err());
}

TEST_F(SharedPtrMakerFixture, MakeSharedArraySharedOwnership) {
  auto result = tmn::make_shared_array<std::string>(2);
  ASSERT_TRUE(result.is_ok());

  auto array1 = result.unwrap_value();
  // result.shared_ptr + array1 = 2;
  EXPECT_EQ(array1.counter_value(), 2);

  auto array2 = array1;
  EXPECT_EQ(array1.counter_value(), 3);
  EXPECT_EQ(array2.counter_value(), 3);

  array1[0] = "First";
  array2[1] = "Second";

  EXPECT_EQ(array1[0], "First");
  EXPECT_EQ(array1[1], "Second");
  EXPECT_EQ(array2[0], "First");
  EXPECT_EQ(array2[1], "Second");
}
