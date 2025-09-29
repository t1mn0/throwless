#include <gtest/gtest.h>

#include "../../SmartPtr/WeakPtr/WeakPtr.hpp"
#include "../_TestUtils/Utils.hpp"

#include <gtest/gtest.h>

class WeakPtrFixture : public ::testing::Test {
protected:
  void SetUp() override {
    tmn::test_utils::WeakTestObject::reset_counts();
    tmn::test_utils::WeakTestDeleter::reset();
    tmn::test_utils::SharedTestObject::reset_counts();
    tmn::test_utils::SharedTestDeleter::reset();
  }

  void TearDown() override {
    tmn::test_utils::WeakTestObject::reset_counts();
    tmn::test_utils::WeakTestDeleter::reset();
    tmn::test_utils::SharedTestObject::reset_counts();
    tmn::test_utils::SharedTestDeleter::reset();
  }
};

TEST_F(WeakPtrFixture, DefaultConstructor) {
  tmn::WeakPtr<tmn::test_utils::WeakTestObject> weak_ptr;

  EXPECT_TRUE(weak_ptr.is_expired());
  EXPECT_EQ(weak_ptr.counter_value(), 0);

  auto shared = weak_ptr.promote();
  EXPECT_FALSE(static_cast<bool>(shared));
  EXPECT_EQ(shared.counter_value(), 0);
}

TEST_F(WeakPtrFixture, ConstructorFromSharedPtr) {
  auto shared = tmn::SharedPtr(new tmn::test_utils::WeakTestObject(42));
  tmn::WeakPtr<tmn::test_utils::WeakTestObject> weak_ptr(shared);

  EXPECT_FALSE(weak_ptr.is_expired());
  EXPECT_EQ(weak_ptr.counter_value(), 1);

  auto promoted = weak_ptr.promote();
  EXPECT_TRUE(static_cast<bool>(promoted));
  EXPECT_EQ(promoted->value, 42);
}

TEST_F(WeakPtrFixture, CopyConstructor) {
  auto shared = tmn::SharedPtr(new tmn::test_utils::WeakTestObject(100));
  tmn::WeakPtr<tmn::test_utils::WeakTestObject> weak1(shared);
  tmn::WeakPtr<tmn::test_utils::WeakTestObject> weak2(weak1);

  EXPECT_EQ(weak1.counter_value(), 1);
  EXPECT_EQ(weak2.counter_value(), 1);
  EXPECT_FALSE(weak1.is_expired());
  EXPECT_FALSE(weak2.is_expired());

  auto promoted1 = weak1.promote();
  auto promoted2 = weak2.promote();
  EXPECT_EQ(promoted1.get(), promoted2.get());
}

TEST_F(WeakPtrFixture, MoveConstructor) {
  auto shared = tmn::SharedPtr(new tmn::test_utils::WeakTestObject(200));
  tmn::WeakPtr<tmn::test_utils::WeakTestObject> weak1(shared);
  tmn::WeakPtr<tmn::test_utils::WeakTestObject> weak2(std::move(weak1));

  EXPECT_TRUE(weak1.is_expired());
  EXPECT_EQ(weak1.counter_value(), 0);
  EXPECT_FALSE(weak2.is_expired());
  EXPECT_EQ(weak2.counter_value(), 1);

  auto promoted = weak2.promote();
  EXPECT_EQ(promoted->value, 200);
}

TEST_F(WeakPtrFixture, CopyAssignment) {
  auto shared = tmn::SharedPtr(new tmn::test_utils::WeakTestObject(300));
  tmn::WeakPtr<tmn::test_utils::WeakTestObject> weak1(shared);
  tmn::WeakPtr<tmn::test_utils::WeakTestObject> weak2;

  weak2 = weak1;

  EXPECT_EQ(weak1.counter_value(), 1);
  EXPECT_EQ(weak2.counter_value(), 1);
  EXPECT_FALSE(weak1.is_expired());
  EXPECT_FALSE(weak2.is_expired());

  EXPECT_EQ(weak1.promote().get(), weak2.promote().get());
}

TEST_F(WeakPtrFixture, MoveAssignment) {
  auto shared = tmn::SharedPtr(new tmn::test_utils::WeakTestObject(400));
  tmn::WeakPtr<tmn::test_utils::WeakTestObject> weak1(shared);
  tmn::WeakPtr<tmn::test_utils::WeakTestObject> weak2;

  weak2 = std::move(weak1);

  EXPECT_TRUE(weak1.is_expired());
  EXPECT_FALSE(weak2.is_expired());
  EXPECT_EQ(weak2.counter_value(), 1);

  auto promoted = weak2.promote();
  EXPECT_EQ(promoted->value, 400);
}

TEST_F(WeakPtrFixture, AssignmentFromSharedPtr) {
  auto shared1 = tmn::SharedPtr(new tmn::test_utils::WeakTestObject(500));
  auto shared2 = tmn::SharedPtr(new tmn::test_utils::WeakTestObject(600));

  tmn::WeakPtr<tmn::test_utils::WeakTestObject> weak_ptr(shared1);
  weak_ptr = shared2;

  EXPECT_EQ(weak_ptr.counter_value(), 1);
  auto promoted = weak_ptr.promote();
  EXPECT_EQ(promoted->value, 600);
}

TEST_F(WeakPtrFixture, ExpiredWhenSharedPtrDestroyed) {
  tmn::WeakPtr<tmn::test_utils::WeakTestObject> weak_ptr;

  {
    auto shared = tmn::SharedPtr(new tmn::test_utils::WeakTestObject(700));
    weak_ptr = shared;

    EXPECT_FALSE(weak_ptr.is_expired());
    EXPECT_EQ(weak_ptr.counter_value(), 1);
  }

  ASSERT_EQ(weak_ptr.counter_value(), 0);

  EXPECT_TRUE(weak_ptr.is_expired());
  EXPECT_EQ(weak_ptr.counter_value(), 0);

  auto promoted = weak_ptr.promote();
  EXPECT_FALSE(static_cast<bool>(promoted));
}

TEST_F(WeakPtrFixture, LockReturnsValidSharedPtr) {
  auto shared = tmn::SharedPtr(new tmn::test_utils::WeakTestObject(800));
  tmn::WeakPtr<tmn::test_utils::WeakTestObject> weak_ptr(shared);

  auto promoted = weak_ptr.promote();

  EXPECT_TRUE(static_cast<bool>(promoted));
  EXPECT_EQ(promoted->value, 800);
  EXPECT_EQ(shared.counter_value(), 2);
  EXPECT_EQ(weak_ptr.counter_value(), 2);
}

TEST_F(WeakPtrFixture, LockReturnsEmptyWhenExpired) {
  tmn::WeakPtr<tmn::test_utils::WeakTestObject> weak_ptr;

  {
    auto shared = tmn::SharedPtr(new tmn::test_utils::WeakTestObject(900));
    weak_ptr = shared;
  }

  auto promoted = weak_ptr.promote();
  EXPECT_FALSE(static_cast<bool>(promoted));
  EXPECT_EQ(promoted.counter_value(), 0);
}

TEST_F(WeakPtrFixture, ResetMakesExpired) {
  auto shared = tmn::SharedPtr(new tmn::test_utils::WeakTestObject(1000));
  tmn::WeakPtr<tmn::test_utils::WeakTestObject> weak_ptr(shared);

  EXPECT_FALSE(weak_ptr.is_expired());

  weak_ptr.reset();

  EXPECT_TRUE(weak_ptr.is_expired());
  EXPECT_EQ(weak_ptr.counter_value(), 0);

  EXPECT_TRUE(static_cast<bool>(shared));
  EXPECT_EQ(shared.counter_value(), 1);
}

TEST_F(WeakPtrFixture, UseCountTracking) {
  tmn::WeakPtr<tmn::test_utils::WeakTestObject> weak_ptr;

  EXPECT_EQ(weak_ptr.counter_value(), 0);

  {
    auto shared1 = tmn::SharedPtr(new tmn::test_utils::WeakTestObject(1100));
    weak_ptr = shared1;

    EXPECT_EQ(weak_ptr.counter_value(), 1);

    {
      auto shared2 = shared1;
      EXPECT_EQ(weak_ptr.counter_value(), 2);
    }

    EXPECT_EQ(weak_ptr.counter_value(), 1);
  }

  EXPECT_EQ(weak_ptr.counter_value(), 0);
}

TEST_F(WeakPtrFixture, EqualityOperators) {
  auto shared1 = tmn::SharedPtr(new tmn::test_utils::WeakTestObject(1200));
  auto shared2 = tmn::SharedPtr(new tmn::test_utils::WeakTestObject(1300));

  tmn::WeakPtr<tmn::test_utils::WeakTestObject> weak1(shared1);
  tmn::WeakPtr<tmn::test_utils::WeakTestObject> weak2(shared1);
  tmn::WeakPtr<tmn::test_utils::WeakTestObject> weak3(shared2);

  EXPECT_EQ(weak1, weak2);
  EXPECT_NE(weak1, weak3);
  EXPECT_NE(weak2, weak3);
}

TEST_F(WeakPtrFixture, MultipleWeakPtrsToSameObject) {
  auto shared = tmn::SharedPtr(new tmn::test_utils::WeakTestObject(1500));

  tmn::WeakPtr<tmn::test_utils::WeakTestObject> weak1(shared);
  tmn::WeakPtr<tmn::test_utils::WeakTestObject> weak2(shared);
  tmn::WeakPtr<tmn::test_utils::WeakTestObject> weak3(shared);

  EXPECT_EQ(weak1.counter_value(), 1);
  EXPECT_EQ(weak2.counter_value(), 1);
  EXPECT_EQ(weak3.counter_value(), 1);

  auto promoted1 = weak1.promote();
  auto promoted2 = weak2.promote();
  auto promoted3 = weak3.promote();

  EXPECT_EQ(promoted1.get(), promoted2.get());
  EXPECT_EQ(promoted2.get(), promoted3.get());
  EXPECT_EQ(shared.counter_value(), 4); // original + 3
}

TEST_F(WeakPtrFixture, FriendSwap) {
  auto shared1 = tmn::SharedPtr(new tmn::test_utils::WeakTestObject(1600));
  auto shared2 = tmn::SharedPtr(new tmn::test_utils::WeakTestObject(1700));

  tmn::WeakPtr<tmn::test_utils::WeakTestObject> weak1(shared1);
  tmn::WeakPtr<tmn::test_utils::WeakTestObject> weak2(shared2);

  auto weak1_original = weak1.promote().get();
  auto weak2_original = weak2.promote().get();

  swap(weak1, weak2);

  EXPECT_EQ(weak1.promote().get(), weak2_original);
  EXPECT_EQ(weak2.promote().get(), weak1_original);
}

TEST_F(WeakPtrFixture, EmptyWeakPtrOperations) {
  tmn::WeakPtr<tmn::test_utils::WeakTestObject> weak_ptr;

  ASSERT_NO_THROW(weak_ptr.reset());
  ASSERT_NO_THROW(weak_ptr.is_expired());
  ASSERT_NO_THROW(weak_ptr.counter_value());
  ASSERT_NO_THROW(weak_ptr.promote());

  auto weak2 = weak_ptr;
  auto weak3 = std::move(weak_ptr);

  EXPECT_TRUE(weak2.is_expired());
  EXPECT_TRUE(weak3.is_expired());
}

TEST_F(WeakPtrFixture, ObjectLifetimeWithWeakPtrs) {
  tmn::WeakPtr<tmn::test_utils::WeakTestObject> weak_ptr;

  {
    auto shared = tmn::SharedPtr(new tmn::test_utils::WeakTestObject(1800));
    weak_ptr = shared;

    EXPECT_EQ(tmn::test_utils::WeakTestObject::constructor_count, 1);
    EXPECT_EQ(tmn::test_utils::WeakTestObject::destructor_count, 0);
  } // shared is being destroyed, the object must be deleted

  EXPECT_EQ(tmn::test_utils::WeakTestObject::destructor_count, 1);
  EXPECT_TRUE(weak_ptr.is_expired());
}

TEST_F(WeakPtrFixture, WeakPtrDoesNotPreventDestruction) {
  tmn::test_utils::WeakTestObject::reset_counts();

  tmn::WeakPtr<tmn::test_utils::WeakTestObject> weak_ptr;

  {
    auto shared = tmn::SharedPtr(new tmn::test_utils::WeakTestObject(1900));
    weak_ptr = shared;

    EXPECT_EQ(tmn::test_utils::WeakTestObject::constructor_count, 1);
    EXPECT_EQ(tmn::test_utils::WeakTestObject::destructor_count, 0);
  }

  // Resource should be deleted despite the existing weak_ptr
  EXPECT_EQ(tmn::test_utils::WeakTestObject::destructor_count, 1);
  EXPECT_TRUE(weak_ptr.is_expired());
}
