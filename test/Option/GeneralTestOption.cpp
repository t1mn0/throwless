#include <gtest/gtest.h>
#include <stdexcept>
#include <string>
#include "../../include/Option/Option.hpp"

TEST(OptionFunctorTest, ConformsToFunctorConcept) {
    static_assert(fpp::Functor<fpp::Option, int,         decltype([](int x)                { return x * 2; })>);
    static_assert(fpp::Functor<fpp::Option, std::string, decltype([](const std::string& s) { return s.length(); })>);
}

TEST(OptionTest, DefaultConstructor) {
    fpp::Option<int> opt;
    EXPECT_FALSE(opt.has_value());
    EXPECT_FALSE(static_cast<bool>(opt));
}

TEST(OptionTest, ValueConstructor) {
    fpp::Option<int> opt(42);
    EXPECT_TRUE(opt.has_value());
    EXPECT_EQ(opt.value_or_exception(), 42);
}

TEST(OptionTest, MoveConstructor) {
    fpp::Option<std::string> opt1("test");
    fpp::Option<std::string> opt2(std::move(opt1));
    EXPECT_TRUE(opt2.has_value());
    EXPECT_EQ(opt2.value_or_exception(), std::string("test"));
    EXPECT_FALSE(opt1.has_value());
}

TEST(OptionTest, CopyConstructor) {
    fpp::Option<int> opt1(42);
    fpp::Option<int> opt2(opt1);
    EXPECT_TRUE(opt2.has_value());
    EXPECT_EQ(opt2.value_or_exception(), 42);
    EXPECT_TRUE(opt1.has_value());
}

TEST(OptionTest, AssignmentOperators) {
    fpp::Option<int> opt1(42);
    fpp::Option<int> opt2;
    opt2 = opt1;
    EXPECT_TRUE(opt2.has_value());
    EXPECT_EQ(opt2.value_or_exception(), 42);

    fpp::Option<int> opt3;
    opt3 = fpp::Option<int>(99);
    EXPECT_TRUE(opt3.has_value());
    EXPECT_EQ(opt3.value_or_exception(), 99);
}

TEST(OptionTest, Swap) {
    fpp::Option<int> opt1(42);
    fpp::Option<int> opt2(99);
    opt1.swap(opt2);
    EXPECT_EQ(opt1.value_or_exception(), 99);
    EXPECT_EQ(opt2.value_or_exception(), 42);
}

TEST(OptionTest, ValueOr) {
    fpp::Option<int> opt1(42);
    fpp::Option<int> opt2;
    
    EXPECT_EQ(opt1.value_or(0), 42);
    EXPECT_EQ(opt2.value_or(0), 0);
    
    const int def = 100;
    EXPECT_EQ(opt1.value_or(def), 42);
    EXPECT_EQ(opt2.value_or(def), 100);
}

TEST(OptionTest, ValueOrDefault) {
    fpp::Option<int> opt1(42);
    fpp::Option<int> opt2;
    
    EXPECT_EQ(opt1.value_or_default(), 42);
    EXPECT_EQ(opt2.value_or_default(), 0);
}

TEST(OptionTest, ValueOrException) {
    fpp::Option<int> opt1(42);
    fpp::Option<int> opt2;
    
    EXPECT_EQ(opt1.value_or_exception(), 42);
    EXPECT_THROW(opt2.value_or_exception(), std::bad_optional_access);
}

TEST(OptionTest, DestroyValue) {
    fpp::Option<std::string> opt("test");
    EXPECT_TRUE(opt.destroy_value());
    EXPECT_FALSE(opt.has_value());
    EXPECT_FALSE(opt.destroy_value());
}

TEST(OptionTest, Map) {
    fpp::Option<int> opt1(42);
    fpp::Option<int> opt2;
    
    auto doubled = opt1.fmap([](int x) { return x * 2; });
    EXPECT_TRUE(doubled.has_value());
    EXPECT_EQ(doubled.value_or_exception(), 84);
    
    auto empty = opt2.fmap([](int x) { return x * 2; });
    EXPECT_FALSE(empty.has_value());
}

TEST(OptionTest, AndThen) {
    fpp::Option<int> opt1(42);
    auto result1 = opt1.and_then([](int x) { return x * 2; });
    EXPECT_TRUE(result1.has_value());
    EXPECT_EQ(result1.value_or_exception(), 84);

    fpp::Option<int> opt2;
    auto result2 = opt2.and_then([](int x) { return x * 2; });
    EXPECT_FALSE(result2.has_value());

    fpp::Option<std::string> opt3("hello");
    auto result3 = opt3.and_then([](const std::string& s) { return s.length(); });
    EXPECT_TRUE(result3.has_value());
    EXPECT_EQ(result3.value_or_exception(), 5);

    fpp::Option<int> opt4(10);
    auto result4 = opt4.and_then([](int x) { 
        return std::to_string(x); 
    });
    EXPECT_TRUE(result4.has_value());
    EXPECT_EQ(result4.value_or_exception(), std::string("10"));
}

TEST(OptionTest, OrElse) {
    fpp::Option<int> opt1(42);
    bool called1 = false;
    auto result1 = opt1.or_else([&called1]() { 
        called1 = true; 
        return fpp::Option<int>(0); 
    });
    EXPECT_TRUE(result1.has_value());
    EXPECT_EQ(result1.value_or_exception(), 42);
    EXPECT_FALSE(called1);

    fpp::Option<int> opt2;
    bool called2 = false;
    auto result2 = opt2.or_else([&called2]() { 
        called2 = true; 
        return fpp::Option<int>(100); 
    });
    EXPECT_TRUE(result2.has_value());
    EXPECT_EQ(result2.value_or_exception(), 100);
    EXPECT_TRUE(called2);

    fpp::Option<std::string> opt3;
    auto result3 = opt3.or_else([]() { 
        return fpp::Option<std::string>("default"); 
    });
    EXPECT_TRUE(result3.has_value());
    EXPECT_EQ(result3.value_or_exception(), std::string("default"));
}

TEST(OptionTest, AndThenChain) {
    auto result = fpp::Option<int>(10)
        .and_then([](int x) { return x * 2; })  // 20
        .and_then([](int x) { return x + 5; })  // 25
        .and_then([](int x) { return std::to_string(x); }); // "25"
    
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result.value_or_exception(), std::string("25"));
}

TEST(OptionTest, OrElseChain) {
    auto result = fpp::Option<int>()
        .or_else([]() { return fpp::Option<int>(); })
        .or_else([]() { return fpp::Option<int>(10); }) // 10
        .or_else([]() { return fpp::Option<int>(20); });
    
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result.value_or_exception(), 10);
}

TEST(OptionTest, MixedChain) {
    auto result = fpp::Option<int>(5)
        .and_then([](int x) { return x % 2 == 0 ? x : x * 2; })
        .or_else([]() { return fpp::Option<int>(10); }) // 10
        .and_then([](int x) { return x * 3; }); // 30
    
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result.value_or_exception(), 30);
}

TEST(OptionEitherConversion, LeftVoid) {
    fpp::Option<int> some(42);
    fpp::Option<int> none;
    
    auto e1 = some.to_either_left();
    EXPECT_TRUE(e1.is_left());
    EXPECT_EQ(e1.left_value_or_exception(), 42);
    
    auto e2 = none.to_either_left();
    EXPECT_TRUE(e2.is_right());
}

TEST(OptionEitherConversion, RightVoid) {
    fpp::Option<std::string> some("test");
    fpp::Option<std::string> none;
    
    auto e1 = some.to_either_right();
    EXPECT_TRUE(e1.is_right());
    EXPECT_EQ(e1.right_value_or_exception(), "test");
    
    auto e2 = none.to_either_right();
    EXPECT_TRUE(e2.is_left());
}

TEST(OptionConversions, ToResult) {
    fpp::Option<int> some(100);
    fpp::Result<int, fpp::StringError> res = some.to_result(fpp::StringError("missing"));
    EXPECT_TRUE(res.is_ok());
    EXPECT_EQ(res.unwrap_or_exception(), 100);
    
    fpp::Option<int> none;
    fpp::Result<int, fpp::StringError> res2 = none.to_result(fpp::StringError("missing"));
    EXPECT_TRUE(res2.is_err());
    EXPECT_EQ(res2.unwrap_err_or_exception().err_message(), "missing");
}