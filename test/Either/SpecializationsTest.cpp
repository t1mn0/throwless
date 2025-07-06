#include <gtest/gtest.h>
#include <stdexcept>
#include <string>
#include "../../include/Either/Either.hpp"

TEST(EitherLVoidTest, ValueConstructors) {
    fpp::Either<int, void> left(42);
    EXPECT_TRUE(left.is_left());
    EXPECT_EQ(left.left_value_or_exception(), 42);

    fpp::Either<int, void> right = fpp::Either<int, void>::from_right();
    EXPECT_TRUE(right.is_right());
    EXPECT_NO_THROW(right.right_value_or_exception());
}

TEST(EitherLVoidTest, MoveConstructors) {
    fpp::Either<std::string, void> left(std::string("test"));
    EXPECT_TRUE(left.is_left());
    EXPECT_EQ(left.left_value_or_exception(), "test");

    fpp::Either<std::string, void> right = fpp::Either<std::string, void>::from_right();
    EXPECT_TRUE(right.is_right());
}

TEST(EitherLVoidTest, CopyOperations) {
    fpp::Either<int, void> original = fpp::Either<int, void>::from_left(42);
    fpp::Either<int, void> copy(original);
    
    EXPECT_TRUE(copy.is_left());
    EXPECT_EQ(copy.left_value_or_exception(), 42);

    fpp::Either<int, void> original_right = fpp::Either<int, void>::from_right();
    fpp::Either<int, void> copy_right(original_right);
    
    EXPECT_TRUE(copy_right.is_right());
}

TEST(EitherLVoidTest, FunctionalMethods) {
    fpp::Either<int, void> left(42);
    auto mapped_left = left.fmap_left([](int x) { return x * 2; });
    EXPECT_TRUE(mapped_left.is_left());
    EXPECT_EQ(mapped_left.left_value_or_exception(), 84);

    fpp::Either<int, void> right = fpp::Either<int, void>::from_right();
    auto mapped_right = right.fmap_left([](int x) { return x * 2; });
    EXPECT_TRUE(mapped_right.is_right());
}

TEST(EitherVoidRTest, ValueConstructors) {
    fpp::Either<void, int> left = fpp::Either<void, int>::from_left();
    EXPECT_TRUE(left.is_left());
    EXPECT_NO_THROW(left.left_value_or_exception());

    fpp::Either<void, int> right(42);
    EXPECT_TRUE(right.is_right());
    EXPECT_EQ(right.right_value_or_exception(), 42);
}

TEST(EitherVoidRTest, MoveConstructors) {
    fpp::Either<void, std::string> left = fpp::Either<void, std::string>::from_left();
    EXPECT_TRUE(left.is_left());

    fpp::Either<void, std::string> right(std::string("error"));
    EXPECT_TRUE(right.is_right());
    EXPECT_EQ(right.right_value_or_exception(), "error");
}

TEST(EitherVoidRTest, CopyOperations) {
    fpp::Either<void, int> original = fpp::Either<void, int>::from_right(42);
    fpp::Either<void, int> copy(original);
    
    EXPECT_TRUE(copy.is_right());
    EXPECT_EQ(copy.right_value_or_exception(), 42);

    fpp::Either<void, int> original_left = fpp::Either<void, int>::from_left();
    fpp::Either<void, int> copy_left(original_left);
    
    EXPECT_TRUE(copy_left.is_left());
}
TEST(EitherVoidRTest, FunctionalMethods) {
    fpp::Either<void, int> right(42);
    auto mapped_right = right.fmap_right([](int x) { return x * 2; });
    EXPECT_TRUE(mapped_right.is_right());
    EXPECT_EQ(mapped_right.right_value_or_exception(), 84);

    fpp::Either<void, int> left = fpp::Either<void, int>::from_left();
    auto mapped_left = left.fmap_right([](int x) { return x * 2; });
    EXPECT_TRUE(mapped_left.is_left());
}

TEST(EitherVoidRTest, Transpose) {
    fpp::Either<int, void> original_left(42);
    auto transposed_left = original_left.transpose_types();
    EXPECT_TRUE(transposed_left.is_right());
    EXPECT_EQ(transposed_left.right_value_or_exception(), 42);

    fpp::Either<int, void> original_right = fpp::Either<int, void>::from_right();
    auto transposed_right = original_right.transpose_types();
    EXPECT_TRUE(transposed_right.is_left());
}

TEST(EitherLVoidTest, SwapOperations) {
    fpp::Either<int, void> a(42);
    fpp::Either<int, void> b = fpp::Either<int, void>::from_right();
    
    a.swap(b);
    
    EXPECT_TRUE(a.is_right());
    EXPECT_TRUE(b.is_left());
    EXPECT_EQ(b.left_value_or_exception(), 42);
}

TEST(EitherVoidRTest, SwapOperations) {
    fpp::Either<void, int> a = fpp::Either<void, int>::from_left();
    fpp::Either<void, int> b(42);
    
    a.swap(b);
    
    EXPECT_TRUE(a.is_right());
    EXPECT_EQ(a.right_value_or_exception(), 42);
    EXPECT_TRUE(b.is_left());
}

TEST(EitherLVoidTest, ValueOr) {
    fpp::Either<int, void> left(42);
    EXPECT_EQ(left.left_value_or(10), 42);
    
    fpp::Either<int, void> right = fpp::Either<int, void>::from_right();
    EXPECT_EQ(right.left_value_or(10), 10);
    
    EXPECT_EQ(right.left_value_or_default(), 0);
}

TEST(EitherVoidRTest, ValueOr) {
    fpp::Either<void, int> right(42);
    EXPECT_EQ(right.right_value_or(10), 42);
    
    fpp::Either<void, int> left = fpp::Either<void, int>::from_left();
    EXPECT_EQ(left.right_value_or(10), 10);
    
    EXPECT_EQ(left.right_value_or_default(), 0);
}