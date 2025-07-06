#include <gtest/gtest.h>
#include <stdexcept>
#include <string>
#include "../../include/Either/Either.hpp"

TEST(EitherTest, ValueConstructors) {
    fpp::Either<int, std::string> left(42);
    EXPECT_TRUE(left.is_left());
    EXPECT_EQ(left.left_value_or_exception(), 42);

    fpp::Either<int, std::string> right("error");
    EXPECT_TRUE(right.is_right());
    EXPECT_EQ(right.right_value_or_exception(), "error");
}

TEST(EitherTest, MoveConstructors) {
    fpp::Either<std::string, int> left(std::string("test"));
    EXPECT_TRUE(left.is_left());
    EXPECT_EQ(left.left_value_or_exception(), "test");

    fpp::Either<std::string, int> right(42);
    EXPECT_TRUE(right.is_right());
    EXPECT_EQ(right.right_value_or_exception(), 42);
}

TEST(EitherTest, CopyOperations) {
    fpp::Either<int, std::string> original = fpp::Either<int, std::string>::from_left(42);
    fpp::Either<int, std::string> copy(original);
    
    EXPECT_TRUE(copy.is_left());
    EXPECT_EQ(copy.left_value_or_exception(), 42);
}

TEST(EitherTest, MoveOperations) {
    fpp::Either<std::string, int> original = fpp::Either<std::string, int>::from_left("test");
    fpp::Either<std::string, int> moved(std::move(original));
    
    EXPECT_TRUE(moved.is_left());
    EXPECT_EQ(moved.left_value_or_exception(), "test");
    EXPECT_TRUE(original.is_left());
}

TEST(EitherTest, FactoryMethods) {
    auto left = fpp::Either<int, std::string>::from_left(42);
    EXPECT_TRUE(left.is_left());
    EXPECT_EQ(left.left_value_or_exception(), 42);

    auto right = fpp::Either<int, std::string>::from_right("error");
    EXPECT_TRUE(right.is_right());
    EXPECT_EQ(right.right_value_or_exception(), "error");
}

TEST(EitherTest, ValueAccess) {
    fpp::Either<int, std::string> left = fpp::Either<int, std::string>::from_left(42);
    EXPECT_EQ(left.left_value().value_or_exception(), 42);
    EXPECT_FALSE(left.right_value().has_value());

    fpp::Either<int, std::string> right = fpp::Either<int, std::string>::from_right("error");
    EXPECT_EQ(right.right_value().value_or_exception(), "error");
    EXPECT_FALSE(right.left_value().has_value());
}

TEST(EitherTest, ValueOr) {
    fpp::Either<int, std::string> left = fpp::Either<int, std::string>::from_left(42);
    EXPECT_EQ(left.left_value_or(0), 42);
    EXPECT_EQ(left.right_value_or("default"), "default");

    fpp::Either<int, std::string> right = fpp::Either<int, std::string>::from_right("error");
    EXPECT_EQ(right.left_value_or(0), 0);
    EXPECT_EQ(right.right_value_or("default"), "error");
}

TEST(EitherTest, ValueOrException) {
    fpp::Either<int, std::string> left = fpp::Either<int, std::string>::from_left(42);
    EXPECT_EQ(left.left_value_or_exception(), 42);
    EXPECT_THROW(left.right_value_or_exception(), std::runtime_error);

    fpp::Either<int, std::string> right = fpp::Either<int, std::string>::from_right("error");
    EXPECT_EQ(right.right_value_or_exception(), "error");
    EXPECT_THROW(right.left_value_or_exception(), std::runtime_error);
}

TEST(EitherTest, Swap) {
    fpp::Either<int, std::string> a = fpp::Either<int, std::string>::from_left(42);
    fpp::Either<int, std::string> b = fpp::Either<int, std::string>::from_right("error");
    
    std::swap(a, b);
    
    EXPECT_TRUE(a.is_right());
    EXPECT_TRUE(b.is_left());
    EXPECT_EQ(a.right_value_or_exception(), "error");
    EXPECT_EQ(b.left_value_or_exception(), 42);
}

TEST(EitherTest, TransposeTypes) {
    fpp::Either<int, std::string> left = fpp::Either<int, std::string>::from_left(42);
    auto transposed_left = left.transpose_types();
    
    EXPECT_TRUE(transposed_left.is_right());
    EXPECT_EQ(transposed_left.right_value_or_exception(), 42);

    fpp::Either<int, std::string> right = fpp::Either<int, std::string>::from_right("error");
    auto transposed_right = right.transpose_types();
    
    EXPECT_TRUE(transposed_right.is_left());
    EXPECT_EQ(transposed_right.left_value_or_exception(), "error");
}

TEST(EitherTest, MapLeft) {
    fpp::Either<int, std::string> left = fpp::Either<int, std::string>::from_left(42);
    auto mapped = left.map_left([](int x) { return x * 2; });
    EXPECT_TRUE(mapped.is_left());
    EXPECT_EQ(mapped.left_value_or_exception(), 84);

    fpp::Either<int, std::string> right = fpp::Either<int, std::string>::from_right("error");
    auto unchanged = right.map_left([](int x) { return x * 2; });
    EXPECT_TRUE(unchanged.is_right());
    EXPECT_EQ(unchanged.right_value_or_exception(), "error");
}

TEST(EitherTest, MapRight) {
    fpp::Either<std::string, int> left = fpp::Either<std::string, int>::from_left("error");
    auto unchanged = left.map_right([](int x) { return x * 2; });
    EXPECT_TRUE(unchanged.is_left());
    EXPECT_EQ(unchanged.left_value_or_exception(), "error");

    fpp::Either<std::string, int> right = fpp::Either<std::string, int>::from_right(42);
    auto mapped = right.map_right([](int x) { return x * 2; });
    EXPECT_TRUE(mapped.is_right());
    EXPECT_EQ(mapped.right_value_or_exception(), 84);
}