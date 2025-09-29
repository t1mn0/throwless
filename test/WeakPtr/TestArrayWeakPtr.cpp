#include <gtest/gtest.h>

#include "../../SmartPtr/WeakPtr/WeakPtr.hpp"
#include "../../SmartPtr/SharedPtr/SharedPtr.hpp"
#include "../_TestUtils/Utils.hpp"

class WeakPtrArrayTest : public ::testing::Test {
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

TEST_F(WeakPtrArrayTest, DefaultConstructor) {
  tmn::WeakPtr<tmn::test_utils::SharedTestObject[]> weak_ptr;

  EXPECT_TRUE(weak_ptr.is_expired());
  EXPECT_EQ(weak_ptr.counter_value(), 0);
  EXPECT_EQ(weak_ptr.size(), 0);

  auto shared = weak_ptr.promote();
  EXPECT_FALSE(static_cast<bool>(shared));
  EXPECT_EQ(shared.counter_value(), 0);
  EXPECT_EQ(shared.size(), 0);
}

TEST_F(WeakPtrArrayTest, ConstructorFromSharedPtrArray) {
  auto shared = tmn::SharedPtr<tmn::test_utils::SharedTestObject[]>(
    new tmn::test_utils::SharedTestObject[3]{10, 20, 30}, 3);

  tmn::WeakPtr<tmn::test_utils::SharedTestObject[]> weak_ptr(shared);

  EXPECT_FALSE(weak_ptr.is_expired());
  EXPECT_EQ(weak_ptr.counter_value(), 1);
  EXPECT_EQ(weak_ptr.size(), 3);

  auto promoted = weak_ptr.promote();
  EXPECT_TRUE(static_cast<bool>(promoted));
  EXPECT_EQ(promoted.counter_value(), 2);
  EXPECT_EQ(promoted.size(), 3);
  EXPECT_EQ(promoted[0].value, 10);
  EXPECT_EQ(promoted[2].value, 30);
}

TEST_F(WeakPtrArrayTest, CopyConstructor) {
  auto shared = tmn::SharedPtr<tmn::test_utils::SharedTestObject[]>(
    new tmn::test_utils::SharedTestObject[2]{100, 200}, 2);

  tmn::WeakPtr<tmn::test_utils::SharedTestObject[]> weak1(shared);
  tmn::WeakPtr<tmn::test_utils::SharedTestObject[]> weak2(weak1);

  EXPECT_EQ(weak1.counter_value(), 1);
  EXPECT_EQ(weak2.counter_value(), 1);
  EXPECT_EQ(weak1.size(), 2);
  EXPECT_EQ(weak2.size(), 2);
  EXPECT_FALSE(weak1.is_expired());
  EXPECT_FALSE(weak2.is_expired());

  auto promoted1 = weak1.promote();
  auto promoted2 = weak2.promote();
  EXPECT_EQ(promoted1.get(), promoted2.get());
  EXPECT_EQ(promoted1.size(), promoted2.size());
}

TEST_F(WeakPtrArrayTest, MoveConstructor) {
  auto shared = tmn::SharedPtr<tmn::test_utils::SharedTestObject[]>(
    new tmn::test_utils::SharedTestObject[2]{300, 400}, 2);

  tmn::WeakPtr<tmn::test_utils::SharedTestObject[]> weak1(shared);
  tmn::WeakPtr<tmn::test_utils::SharedTestObject[]> weak2(std::move(weak1));

  EXPECT_TRUE(weak1.is_expired());
  EXPECT_EQ(weak1.counter_value(), 0);
  EXPECT_EQ(weak1.size(), 0);

  EXPECT_FALSE(weak2.is_expired());
  EXPECT_EQ(weak2.counter_value(), 1);
  EXPECT_EQ(weak2.size(), 2);

  auto promoted = weak2.promote();
  EXPECT_EQ(promoted[0].value, 300);
  EXPECT_EQ(promoted[1].value, 400);
}

TEST_F(WeakPtrArrayTest, CopyAssignment) {
  auto shared = tmn::SharedPtr<tmn::test_utils::SharedTestObject[]>(
    new tmn::test_utils::SharedTestObject[3]{500, 600, 700}, 3);

  tmn::WeakPtr<tmn::test_utils::SharedTestObject[]> weak1(shared);
  tmn::WeakPtr<tmn::test_utils::SharedTestObject[]> weak2;

  weak2 = weak1;

  EXPECT_EQ(weak1.counter_value(), 1);
  EXPECT_EQ(weak2.counter_value(), 1);
  EXPECT_EQ(weak1.size(), 3);
  EXPECT_EQ(weak2.size(), 3);
  EXPECT_FALSE(weak1.is_expired());
  EXPECT_FALSE(weak2.is_expired());

  EXPECT_EQ(weak1.promote().get(), weak2.promote().get());
}

TEST_F(WeakPtrArrayTest, MoveAssignment) {
  auto shared = tmn::SharedPtr<tmn::test_utils::SharedTestObject[]>(
    new tmn::test_utils::SharedTestObject[2]{800, 900}, 2);

  tmn::WeakPtr<tmn::test_utils::SharedTestObject[]> weak1(shared);
  tmn::WeakPtr<tmn::test_utils::SharedTestObject[]> weak2;

  weak2 = std::move(weak1);

  EXPECT_TRUE(weak1.is_expired());
  EXPECT_EQ(weak1.counter_value(), 0);
  EXPECT_EQ(weak1.size(), 0);

  EXPECT_FALSE(weak2.is_expired());
  EXPECT_EQ(weak2.counter_value(), 1);
  EXPECT_EQ(weak2.size(), 2);

  auto promoted = weak2.promote();
  EXPECT_EQ(promoted[0].value, 800);
  EXPECT_EQ(promoted[1].value, 900);
}

TEST_F(WeakPtrArrayTest, AssignmentFromSharedPtrArray) {
  auto shared1 = tmn::SharedPtr<tmn::test_utils::SharedTestObject[]>(
    new tmn::test_utils::SharedTestObject[2]{1000, 1100}, 2);
  auto shared2 = tmn::SharedPtr<tmn::test_utils::SharedTestObject[]>(
    new tmn::test_utils::SharedTestObject[3]{1200, 1300, 1400}, 3);

  tmn::WeakPtr<tmn::test_utils::SharedTestObject[]> weak_ptr(shared1);
  weak_ptr = shared2;

  EXPECT_EQ(weak_ptr.counter_value(), 1);
  EXPECT_EQ(weak_ptr.size(), 3);

  auto promoted = weak_ptr.promote();
  EXPECT_EQ(promoted.size(), 3);
  EXPECT_EQ(promoted[0].value, 1200);
  EXPECT_EQ(promoted[2].value, 1400);
}

TEST_F(WeakPtrArrayTest, ExpiredWhenSharedPtrArrayDestroyed) {
  tmn::WeakPtr<tmn::test_utils::SharedTestObject[]> weak_ptr;

  {
    auto shared = tmn::SharedPtr<tmn::test_utils::SharedTestObject[]>(
      new tmn::test_utils::SharedTestObject[2]{1500, 1600}, 2);
    weak_ptr = shared;

    EXPECT_FALSE(weak_ptr.is_expired());
    EXPECT_EQ(weak_ptr.counter_value(), 1);
    EXPECT_EQ(weak_ptr.size(), 2);
  }

  EXPECT_TRUE(weak_ptr.is_expired());
  EXPECT_EQ(weak_ptr.counter_value(), 0);
  // Size should be 0 after destroying the SharedPtr:
  EXPECT_EQ(weak_ptr.size(), 0);

  auto promoted = weak_ptr.promote();
  EXPECT_FALSE(static_cast<bool>(promoted));
  EXPECT_EQ(promoted.size(), 0);
}

TEST_F(WeakPtrArrayTest, LockReturnsValidSharedPtrArray) {
  auto shared = tmn::SharedPtr<tmn::test_utils::SharedTestObject[]>(
    new tmn::test_utils::SharedTestObject[3]{1700, 1800, 1900}, 3);

  tmn::WeakPtr<tmn::test_utils::SharedTestObject[]> weak_ptr(shared);

  auto promoted = weak_ptr.promote();

  EXPECT_TRUE(static_cast<bool>(promoted));
  EXPECT_EQ(promoted[0].value, 1700);
  EXPECT_EQ(promoted[1].value, 1800);
  EXPECT_EQ(promoted.size(), 3);
  EXPECT_EQ(shared.counter_value(), 2);
  EXPECT_EQ(weak_ptr.counter_value(), 2);
}

TEST_F(WeakPtrArrayTest, LockReturnsEmptyWhenExpired) {
  tmn::WeakPtr<tmn::test_utils::SharedTestObject[]> weak_ptr;

  {
    auto shared = tmn::SharedPtr<tmn::test_utils::SharedTestObject[]>(
      new tmn::test_utils::SharedTestObject[2]{2000, 2100}, 2);
    weak_ptr = shared;
  }

  auto promoted = weak_ptr.promote();
  EXPECT_FALSE(static_cast<bool>(promoted));
  EXPECT_EQ(promoted.counter_value(), 0);
  EXPECT_EQ(promoted.size(), 0);
}

TEST_F(WeakPtrArrayTest, ResetMakesExpired) {
  auto shared = tmn::SharedPtr<tmn::test_utils::SharedTestObject[]>(
    new tmn::test_utils::SharedTestObject[2]{2200, 2300}, 2);

  tmn::WeakPtr<tmn::test_utils::SharedTestObject[]> weak_ptr(shared);

  EXPECT_FALSE(weak_ptr.is_expired());
  EXPECT_EQ(weak_ptr.size(), 2);

  weak_ptr.reset();

  EXPECT_TRUE(weak_ptr.is_expired());
  EXPECT_EQ(weak_ptr.counter_value(), 0);
  EXPECT_EQ(weak_ptr.size(), 0);

  // Original SharedPtr should still be valid:
  EXPECT_TRUE(static_cast<bool>(shared));
  EXPECT_EQ(shared.counter_value(), 1);
  EXPECT_EQ(shared.size(), 2);
}

TEST_F(WeakPtrArrayTest, UseCountTrackingWithArrays) {
  tmn::WeakPtr<tmn::test_utils::SharedTestObject[]> weak_ptr;

  EXPECT_EQ(weak_ptr.counter_value(), 0);
  EXPECT_EQ(weak_ptr.size(), 0);

  {
    auto shared1 = tmn::SharedPtr<tmn::test_utils::SharedTestObject[]>(
      new tmn::test_utils::SharedTestObject[2]{2400, 2500}, 2);
    weak_ptr = shared1;

    EXPECT_EQ(weak_ptr.counter_value(), 1);
    EXPECT_EQ(weak_ptr.size(), 2);

    {
      auto shared2 = shared1;
      EXPECT_EQ(weak_ptr.counter_value(), 2);
      EXPECT_EQ(weak_ptr.size(), 2);
    }

    EXPECT_EQ(weak_ptr.counter_value(), 1);
    EXPECT_EQ(weak_ptr.size(), 2);
  }

  EXPECT_EQ(weak_ptr.counter_value(), 0);
  EXPECT_EQ(weak_ptr.size(), 0);
}

TEST_F(WeakPtrArrayTest, SizeMethod) {
  auto shared = tmn::SharedPtr<tmn::test_utils::SharedTestObject[]>(
    new tmn::test_utils::SharedTestObject[5]{1, 2, 3, 4, 5}, 5);

  tmn::WeakPtr<tmn::test_utils::SharedTestObject[]> weak_ptr(shared);

  EXPECT_EQ(weak_ptr.size(), 5);

  shared.reset();
  EXPECT_EQ(weak_ptr.size(), 0);
}

TEST_F(WeakPtrArrayTest, EqualityOperators) {
  auto shared1 = tmn::SharedPtr<tmn::test_utils::SharedTestObject[]>(
    new tmn::test_utils::SharedTestObject[2]{2600, 2700}, 2);
  auto shared2 = tmn::SharedPtr<tmn::test_utils::SharedTestObject[]>(
    new tmn::test_utils::SharedTestObject[2]{2800, 2900}, 2);

  tmn::WeakPtr<tmn::test_utils::SharedTestObject[]> weak1(shared1);
  tmn::WeakPtr<tmn::test_utils::SharedTestObject[]> weak2(shared1);
  tmn::WeakPtr<tmn::test_utils::SharedTestObject[]> weak3(shared2);

  EXPECT_EQ(weak1, weak2);
  EXPECT_NE(weak1, weak3);
  EXPECT_NE(weak2, weak3);
}

TEST_F(WeakPtrArrayTest, MultipleWeakPtrsToSameArray) {
  auto shared = tmn::SharedPtr<tmn::test_utils::SharedTestObject[]>(
    new tmn::test_utils::SharedTestObject[4]{3300, 3400, 3500, 3600}, 4);

  tmn::WeakPtr<tmn::test_utils::SharedTestObject[]> weak1(shared);
  tmn::WeakPtr<tmn::test_utils::SharedTestObject[]> weak2(shared);
  tmn::WeakPtr<tmn::test_utils::SharedTestObject[]> weak3(shared);

  EXPECT_EQ(weak1.counter_value(), 1);
  EXPECT_EQ(weak2.counter_value(), 1);
  EXPECT_EQ(weak3.counter_value(), 1);
  EXPECT_EQ(weak1.size(), 4);
  EXPECT_EQ(weak2.size(), 4);
  EXPECT_EQ(weak3.size(), 4);

  auto promoted1 = weak1.promote();
  auto promoted2 = weak2.promote();
  auto promoted3 = weak3.promote();

  EXPECT_EQ(promoted1.get(), promoted2.get());
  EXPECT_EQ(promoted2.get(), promoted3.get());
  EXPECT_EQ(promoted1.size(), 4);
  EXPECT_EQ(shared.counter_value(), 4);
}

TEST_F(WeakPtrArrayTest, FriendSwap) {
  auto shared1 = tmn::SharedPtr<tmn::test_utils::SharedTestObject[]>(
    new tmn::test_utils::SharedTestObject[2]{3700, 3800}, 2);
  auto shared2 = tmn::SharedPtr<tmn::test_utils::SharedTestObject[]>(
    new tmn::test_utils::SharedTestObject[3]{3900, 4000, 4100}, 3);

  tmn::WeakPtr<tmn::test_utils::SharedTestObject[]> weak1(shared1);
  tmn::WeakPtr<tmn::test_utils::SharedTestObject[]> weak2(shared2);

  auto weak1_original_ptr = weak1.promote().get();
  auto weak2_original_ptr = weak2.promote().get();
  auto weak1_original_size = weak1.size();
  auto weak2_original_size = weak2.size();

  swap(weak1, weak2);

  EXPECT_EQ(weak1.promote().get(), weak2_original_ptr);
  EXPECT_EQ(weak2.promote().get(), weak1_original_ptr);
  EXPECT_EQ(weak1.size(), weak2_original_size);
  EXPECT_EQ(weak2.size(), weak1_original_size);
}

TEST_F(WeakPtrArrayTest, EmptyWeakPtrArrayOperations) {
  tmn::WeakPtr<tmn::test_utils::SharedTestObject[]> weak_ptr;

  ASSERT_NO_THROW(weak_ptr.reset());
  ASSERT_NO_THROW(weak_ptr.is_expired());
  ASSERT_NO_THROW(weak_ptr.counter_value());
  ASSERT_NO_THROW(weak_ptr.size());
  ASSERT_NO_THROW(weak_ptr.promote());

  auto weak2 = weak_ptr;
  auto weak3 = std::move(weak_ptr);

  EXPECT_TRUE(weak2.is_expired());
  EXPECT_TRUE(weak3.is_expired());
  EXPECT_EQ(weak2.size(), 0);
  EXPECT_EQ(weak3.size(), 0);
}

TEST_F(WeakPtrArrayTest, ArrayLifetimeWithWeakPtrs) {
  tmn::WeakPtr<tmn::test_utils::SharedTestObject[]> weak_ptr;

  {
    auto shared = tmn::SharedPtr<tmn::test_utils::SharedTestObject[]>(
      new tmn::test_utils::SharedTestObject[3]{4200, 4300, 4400}, 3);
    weak_ptr = shared;

    EXPECT_EQ(tmn::test_utils::SharedTestObject::constructor_count, 3);
    EXPECT_EQ(tmn::test_utils::SharedTestObject::destructor_count, 0);
  } // shared is being destroyed, the array must be deleted;

  EXPECT_EQ(tmn::test_utils::SharedTestObject::destructor_count, 3);
  EXPECT_TRUE(weak_ptr.is_expired());
  EXPECT_EQ(weak_ptr.size(), 0);
}

TEST_F(WeakPtrArrayTest, WeakPtrDoesNotPreventArrayDestruction) {
  tmn::test_utils::SharedTestObject::reset_counts();

  tmn::WeakPtr<tmn::test_utils::SharedTestObject[]> weak_ptr;

  {
    auto shared = tmn::SharedPtr<tmn::test_utils::SharedTestObject[]>(
      new tmn::test_utils::SharedTestObject[2]{4500, 4600}, 2);
    weak_ptr = shared;

    EXPECT_EQ(tmn::test_utils::SharedTestObject::constructor_count, 2);
    EXPECT_EQ(tmn::test_utils::SharedTestObject::destructor_count, 0);
  }

  // array must be deleted despite the existing WeakPtr
  EXPECT_EQ(tmn::test_utils::SharedTestObject::destructor_count, 2);
  EXPECT_TRUE(weak_ptr.is_expired());
  EXPECT_EQ(weak_ptr.size(), 0);
}

TEST_F(WeakPtrArrayTest, WeakPtrWithCustomArrayDeleter) {
  tmn::test_utils::SharedTestArrayDeleter::reset();

  tmn::WeakPtr<tmn::test_utils::SharedTestObject[]> weak_ptr;

  {
    auto shared = tmn::SharedPtr<tmn::test_utils::SharedTestObject[]>(
      new tmn::test_utils::SharedTestObject[2]{4700, 4800}, 2,
      tmn::test_utils::SharedTestArrayDeleter());

    weak_ptr = shared;

    EXPECT_FALSE(weak_ptr.is_expired());
    EXPECT_EQ(weak_ptr.size(), 2);
  }

  EXPECT_EQ(tmn::test_utils::SharedTestArrayDeleter::delete_count, 1);
  EXPECT_EQ(tmn::test_utils::SharedTestArrayDeleter::last_deleted_size, 2);
  EXPECT_TRUE(weak_ptr.is_expired());
}

TEST_F(WeakPtrArrayTest, LockModifiesArrayThroughMultipleReferences) {
  auto shared = tmn::SharedPtr<tmn::test_utils::SharedTestObject[]>(
    new tmn::test_utils::SharedTestObject[3]{4900, 5000, 5100}, 3);

  tmn::WeakPtr<tmn::test_utils::SharedTestObject[]> weak_ptr(shared);

  auto promoted1 = weak_ptr.promote();
  auto promoted2 = weak_ptr.promote();

  promoted1[1].value = 9999;
  EXPECT_EQ(promoted2[1].value, 9999);
  EXPECT_EQ(shared[1].value, 9999);

  promoted2[0].value = 8888;
  EXPECT_EQ(promoted1[0].value, 8888);
  EXPECT_EQ(shared[0].value, 8888);
}
