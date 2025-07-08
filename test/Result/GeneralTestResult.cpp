#include <gtest/gtest.h>
#include <stdexcept>
#include <string>
#include "../../include/Result/Result.hpp"

struct TestError {
    std::string message;
    int code;

    TestError() = delete;
    TestError(std::string msg, int code = 0) : message(msg), code(code) {};
    TestError(const TestError& oth) = default;
    TestError& operator=(const TestError& oth) = default; 
    TestError& operator=(TestError&& oth) = default; 
    TestError(TestError&& oth) = default;
    ~TestError() = default;

    bool operator==(const TestError& other) const noexcept = default;
    
    std::string err_message() const noexcept { return message; }
    int error_code() const noexcept { return code; }
};

static_assert(fpp::Error<TestError>, "TestError must satisfy Error concept");

TEST(ResultTest, BasicConstruction) {
    auto ok_int = fpp::Result<int, TestError>::Ok(42);
    EXPECT_TRUE(ok_int.is_ok());
    EXPECT_FALSE(ok_int.is_err());
    EXPECT_EQ(ok_int.unwrap_or_exception(), 42);

    auto err = fpp::Result<int, TestError>::Err(TestError{"Failed", 42});
    EXPECT_FALSE(err.is_ok());
    EXPECT_TRUE(err.is_err());
}

TEST(ResultTest, CopyAndMoveSemantics) {
    auto original = fpp::Result<std::string, TestError>::Ok("test");
    auto copy = original;
    EXPECT_EQ(original.unwrap_or_exception(), copy.unwrap_or_exception());

    auto moved = std::move(original);
    EXPECT_EQ(moved.unwrap_or_exception(), "test");

    auto original_err = fpp::Result<int, TestError>::Err(TestError{"Error", 1});
    auto copy_err = original_err;

    auto moved_err = std::move(original_err);
}

TEST(ResultTest, UnwrapMethods) {
    auto ok = fpp::Result<int, TestError>::Ok(10);
    EXPECT_EQ(ok.unwrap().value_or_exception(), 10);

    auto err = fpp::Result<int, TestError>::Err(TestError{"Error", 2});
    EXPECT_THROW(err.unwrap_or_exception(), std::runtime_error);

    auto default_val = 100;
    EXPECT_EQ(err.unwrap_or(default_val), default_val);
    EXPECT_EQ(ok.unwrap_or(default_val), 10);

    auto ok_default = fpp::Result<int, TestError>::Ok(0);
    EXPECT_EQ(ok_default.unwrap_or_default(), 0);
}

TEST(ResultTest, FunctionalMethods) {
    auto ok = fpp::Result<int, TestError>::Ok(2);
    auto squared = ok.fmap([](int x) { return x * x; });
    EXPECT_EQ(squared.unwrap_or_exception(), 4);

    auto err = fpp::Result<int, TestError>::Err(TestError{"Error", 3});
    auto mapped_err = err.fmap([](int x) { return x * x; });
    EXPECT_TRUE(mapped_err.is_err());

    auto chained = ok.and_then([](int x) {
        return std::to_string(x * 2);
    });
    auto q = chained.unwrap_or_exception();
    EXPECT_EQ(q, "4");
}

TEST(ResultTest, SwapTest) {
    auto ok1 = fpp::Result<int, TestError>::Ok(1);
    auto ok2 = fpp::Result<int, TestError>::Ok(2);
    ok1.swap(ok2);
    EXPECT_EQ(ok1.unwrap_or_exception(), 2);
    EXPECT_EQ(ok2.unwrap_or_exception(), 1);

    auto err = fpp::Result<int, TestError>::Err(TestError{"Error", 6});
    ok1.swap(err);
    EXPECT_TRUE(ok1.is_err());
    EXPECT_TRUE(err.is_ok());
    EXPECT_EQ(err.unwrap_or_exception(), 2);
}

TEST(ResultTest, BoolConversion) {
    auto ok = fpp::Result<int, TestError>::Ok(1);
    EXPECT_TRUE(static_cast<bool>(ok));

    auto err = fpp::Result<int, TestError>::Err(TestError{"Error", 7});
    EXPECT_FALSE(static_cast<bool>(err));
}

TEST(ResultConversions, ToEither) {
    fpp::Result<int, fpp::StringError> ok = fpp::Result<int, fpp::StringError>::Ok(42);
    fpp::Either<int, fpp::StringError> e = ok.to_either();
    EXPECT_TRUE(e.is_left());
    EXPECT_EQ(e.left_value_or_exception(), 42);
    
    fpp::Result<int, fpp::StringError> err = fpp::Result<int, fpp::StringError>::Err(fpp::StringError("failed"));
    fpp::Either<int, fpp::StringError> e2 = err.to_either();
    EXPECT_TRUE(e2.is_right());
    EXPECT_EQ(e2.right_value_or_exception().err_message(), "failed");
}

TEST(ResultCustomTypeTest, WorksWithCustomTypes) {
    struct Point {
        int x, y;
        Point operator*(int scalar) const { return Point{x * scalar, y * scalar}; };
    };

    auto ok_point = fpp::Result<Point, TestError>::Ok(Point{1, 2});
    auto scaled = ok_point.fmap([](Point p) { return p * 2; });
    EXPECT_EQ(scaled.unwrap_or_exception().x, 2);
    EXPECT_EQ(scaled.unwrap_or_exception().y, 4);
}