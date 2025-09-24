
#include <gtest/gtest.h>

#include "../../SmartPtr/SharedPtr/SharedPtr.hpp"
#include "../Utils.hpp"

class SharedPtrFixture : public ::testing::Test {
protected:
  void SetUp() override {
    tmn::test_utils::SharedTestObject::reset_counts();
    tmn::test_utils::SharedTestDeleter::reset();
  }

  void TearDown() override {
    tmn::test_utils::SharedTestObject::reset_counts();
    tmn::test_utils::SharedTestDeleter::reset();
  }
};

TEST_F(SharedPtrFixture, DefaultConstructor) {
  tmn::SharedPtr<tmn::test_utils::SharedTestObject> ptr;

  EXPECT_FALSE(static_cast<bool>(ptr));
  EXPECT_EQ(ptr.get(), nullptr);
  EXPECT_EQ(ptr.use_count(), 0);
  EXPECT_FALSE(ptr.try_get().has_value());
}

TEST_F(SharedPtrFixture, ConstructorWithPointer) {
  auto* raw_ptr = new tmn::test_utils::SharedTestObject(42);
  tmn::SharedPtr<tmn::test_utils::SharedTestObject> ptr(raw_ptr);

  EXPECT_TRUE(static_cast<bool>(ptr));
  EXPECT_EQ(ptr.get()->value, 42);
  EXPECT_EQ(ptr.use_count(), 1);
  EXPECT_TRUE(ptr.is_unique());
  EXPECT_EQ(ptr.try_get().value()->value, 42);
}

TEST_F(SharedPtrFixture, ConstructorWithCustomDeleter) {
  auto* raw_ptr = new tmn::test_utils::SharedTestObject(100);
  tmn::SharedPtr<tmn::test_utils::SharedTestObject> ptr(raw_ptr, tmn::test_utils::SharedTestDeleter{});

  EXPECT_EQ(ptr.get()->value, 100);
  EXPECT_EQ(tmn::test_utils::SharedTestDeleter::delete_count, 0);
}

TEST_F(SharedPtrFixture, CopyConstructor) {
  auto ptr1 = tmn::SharedPtr(new tmn::test_utils::SharedTestObject(50));
  auto ptr2 = ptr1;

  EXPECT_EQ(ptr1.use_count(), 2);
  EXPECT_EQ(ptr2.use_count(), 2);
  EXPECT_EQ(ptr1.get(), ptr2.get());
  EXPECT_EQ(ptr1.get()->value, 50);
  EXPECT_EQ(ptr2.get()->value, 50);
  EXPECT_FALSE(ptr1.is_unique());
  EXPECT_FALSE(ptr2.is_unique());
}

TEST_F(SharedPtrFixture, CopyAssignment) {
  auto ptr1 = tmn::SharedPtr(new tmn::test_utils::SharedTestObject(10));
  tmn::SharedPtr<tmn::test_utils::SharedTestObject> ptr2;

  ptr2 = ptr1;

  EXPECT_EQ(ptr1.use_count(), 2);
  EXPECT_EQ(ptr2.use_count(), 2);
  EXPECT_EQ(ptr1.get(), ptr2.get());
}

TEST_F(SharedPtrFixture, MoveConstructor) {
  auto ptr1 = tmn::SharedPtr(new tmn::test_utils::SharedTestObject(30));
  auto original_ptr = ptr1.get();

  auto ptr2 = std::move(ptr1);

  EXPECT_FALSE(static_cast<bool>(ptr1));
  EXPECT_EQ(ptr1.use_count(), 0);
  EXPECT_TRUE(static_cast<bool>(ptr2));
  EXPECT_EQ(ptr2.use_count(), 1);
  EXPECT_EQ(ptr2.get(), original_ptr);
  EXPECT_TRUE(ptr2.is_unique());
}

TEST_F(SharedPtrFixture, MoveAssignment) {
  auto ptr1 = tmn::SharedPtr(new tmn::test_utils::SharedTestObject(40));
  auto ptr2 = tmn::SharedPtr(new tmn::test_utils::SharedTestObject(60));
  auto original_ptr = ptr1.get();

  ptr2 = std::move(ptr1);

  EXPECT_FALSE(static_cast<bool>(ptr1));
  EXPECT_TRUE(static_cast<bool>(ptr2));
  EXPECT_EQ(ptr2.get(), original_ptr);
  EXPECT_EQ(ptr2.use_count(), 1);
}

TEST_F(SharedPtrFixture, ResetToNull) {
  auto ptr = tmn::SharedPtr(new tmn::test_utils::SharedTestObject(70));
  ptr.reset();

  EXPECT_FALSE(static_cast<bool>(ptr));
  EXPECT_EQ(ptr.use_count(), 0);
  EXPECT_EQ(ptr.get(), nullptr);
}

TEST_F(SharedPtrFixture, ResetToNewPointer) {
  auto ptr = tmn::SharedPtr(new tmn::test_utils::SharedTestObject(80));
  auto* new_ptr = new tmn::test_utils::SharedTestObject(90);

  ptr.reset(new_ptr);

  EXPECT_TRUE(static_cast<bool>(ptr));
  EXPECT_EQ(ptr.use_count(), 1);
  EXPECT_EQ(ptr.get()->value, 90);
}

TEST_F(SharedPtrFixture, ResetWithCustomDeleter) {
  auto ptr = tmn::SharedPtr(new tmn::test_utils::SharedTestObject(100));
  auto* new_ptr = new tmn::test_utils::SharedTestObject(200);

  ptr.reset(new_ptr, tmn::test_utils::SharedTestDeleter{});

  EXPECT_EQ(ptr.get()->value, 200);
}

TEST_F(SharedPtrFixture, EqualityOperators) {
  auto ptr1 = tmn::SharedPtr(new tmn::test_utils::SharedTestObject(1));
  auto ptr2 = ptr1;
  auto ptr3 = tmn::SharedPtr(new tmn::test_utils::SharedTestObject(1));

  EXPECT_EQ(ptr1, ptr2);
  EXPECT_NE(ptr1, ptr3);
  EXPECT_NE(ptr2, ptr3);
}

TEST_F(SharedPtrFixture, ComparisonWithNullptr) {
  tmn::SharedPtr<tmn::test_utils::SharedTestObject> null_ptr;
  auto valid_ptr = tmn::SharedPtr(new tmn::test_utils::SharedTestObject(1));

  EXPECT_TRUE(null_ptr == nullptr);
  EXPECT_TRUE(nullptr == null_ptr);
  EXPECT_TRUE(valid_ptr != nullptr);
  EXPECT_TRUE(nullptr != valid_ptr);
}

TEST_F(SharedPtrFixture, RelationalOperators) {
  auto obj1 = new tmn::test_utils::SharedTestObject(1);
  auto obj2 = new tmn::test_utils::SharedTestObject(2);

  tmn::SharedPtr<tmn::test_utils::SharedTestObject> ptr1(obj1);
  tmn::SharedPtr<tmn::test_utils::SharedTestObject> ptr2(obj2);

  if (obj1 < obj2) {
    EXPECT_LT(ptr1, ptr2);
    EXPECT_GT(ptr2, ptr1);
  }
  else {
    EXPECT_LT(ptr2, ptr1);
    EXPECT_GT(ptr1, ptr2);
  }
}

TEST_F(SharedPtrFixture, ResourceDestruction) {
  {
    auto ptr = tmn::SharedPtr(new tmn::test_utils::SharedTestObject(300));
    EXPECT_EQ(tmn::test_utils::SharedTestObject::constructor_count, 1);
    EXPECT_EQ(tmn::test_utils::SharedTestObject::destructor_count, 0);
  }

  EXPECT_EQ(tmn::test_utils::SharedTestObject::destructor_count, 1);
}

TEST_F(SharedPtrFixture, MultipleOwnersDestruction) {
  auto* raw_ptr = new tmn::test_utils::SharedTestObject(400);

  {
    tmn::SharedPtr<tmn::test_utils::SharedTestObject> ptr1(raw_ptr);

    {
      tmn::SharedPtr<tmn::test_utils::SharedTestObject> ptr2 = ptr1;
      EXPECT_EQ(ptr1.use_count(), 2);
      EXPECT_EQ(tmn::test_utils::SharedTestObject::destructor_count, 0);
    } // ptr2 destroyed, but object lives on

    EXPECT_EQ(ptr1.use_count(), 1);
    EXPECT_EQ(tmn::test_utils::SharedTestObject::destructor_count, 0);
  } // ptr1 destroyed, object deleted

  EXPECT_EQ(tmn::test_utils::SharedTestObject::destructor_count, 1);
}

TEST_F(SharedPtrFixture, FriendSwap) {
  auto ptr1 = tmn::SharedPtr(new tmn::test_utils::SharedTestObject(700));
  auto ptr2 = tmn::SharedPtr(new tmn::test_utils::SharedTestObject(800));

  auto ptr1_original = ptr1.get();
  auto ptr2_original = ptr2.get();

  swap(ptr1, ptr2);

  EXPECT_EQ(ptr1.get(), ptr2_original);
  EXPECT_EQ(ptr2.get(), ptr1_original);
}

TEST_F(SharedPtrFixture, EmptySharedPtrOperations) {
  tmn::SharedPtr<tmn::test_utils::SharedTestObject> ptr;

  ASSERT_NO_THROW(ptr.reset());
  ASSERT_NO_THROW(ptr.get());
  ASSERT_NO_THROW(ptr.use_count());
  ASSERT_NO_THROW(ptr.is_unique());
  ASSERT_NO_THROW(ptr.try_get());

  auto ptr2 = ptr;
  auto ptr3 = std::move(ptr);

  EXPECT_FALSE(static_cast<bool>(ptr2));
  EXPECT_FALSE(static_cast<bool>(ptr3));
}

TEST_F(SharedPtrFixture, TryGetFunctionality) {
  tmn::SharedPtr<tmn::test_utils::SharedTestObject> null_ptr;
  auto valid_ptr = tmn::SharedPtr(new tmn::test_utils::SharedTestObject(900));

  EXPECT_TRUE(!null_ptr.try_get().has_value());
  EXPECT_TRUE(valid_ptr.try_get().has_value());
  EXPECT_EQ(valid_ptr.try_get().value()->value, 900);
}

TEST_F(SharedPtrFixture, CustomDeleterCalled) {
  {
    auto* raw_ptr = new tmn::test_utils::SharedTestObject(1000);
    tmn::SharedPtr<tmn::test_utils::SharedTestObject> ptr(raw_ptr, tmn::test_utils::SharedTestDeleter{});

    EXPECT_EQ(tmn::test_utils::SharedTestDeleter::delete_count, 0);
  }

  EXPECT_EQ(tmn::test_utils::SharedTestDeleter::delete_count, 1);
  EXPECT_EQ(tmn::test_utils::SharedTestDeleter::last_deleted_value, 1000);
}
