#include <gtest/gtest.h>

#include <string>

#include "../../Option/Option.hpp"
#include "../../Error/Error.hpp"
#include "../_TestUtils/RandomGenerator.hpp"

class OptionDefaultConstructorFixture : public ::testing::Test {
protected:
  tmn::Option<int> default_opt;
};

TEST_F(OptionDefaultConstructorFixture, DefaultConstructor) {
  EXPECT_FALSE(default_opt.has_value());
  EXPECT_FALSE(static_cast<bool>(default_opt));
}

class OptionValueConstructorFixture : public ::testing::Test {
protected:
  tmn::test_utils::RandomTestData test_data;
  tmn::Option<int> int_opt{test_data.random_int_1};
  tmn::Option<double> double_opt{test_data.random_double_1};
  tmn::Option<std::string> string_opt{test_data.random_string};
};

TEST_F(OptionValueConstructorFixture, ValueConstructorWithRandomValues) {
  ASSERT_TRUE(int_opt.has_value());
  EXPECT_EQ(int_opt.value(), test_data.random_int_1);

  ASSERT_TRUE(double_opt.has_value());
  EXPECT_DOUBLE_EQ(double_opt.value(), test_data.random_double_1);

  ASSERT_TRUE(string_opt.has_value());
  EXPECT_EQ(string_opt.value(), test_data.random_string);
}

class OptionMoveConstructorFixture : public ::testing::Test {
protected:
  tmn::test_utils::RandomTestData test_data;
  tmn::Option<std::string> original_opt{test_data.random_string};
};

TEST_F(OptionMoveConstructorFixture, MoveConstructorWithRandomValue) {
  tmn::Option<std::string> moved_opt(std::move(original_opt));

  ASSERT_TRUE(moved_opt.has_value());
  EXPECT_EQ(moved_opt.value(), test_data.random_string);
  EXPECT_FALSE(original_opt.has_value());
}

class OptionCopyConstructorFixture : public ::testing::Test {
protected:
  tmn::test_utils::RandomTestData test_data;
  tmn::Option<int> original_opt{test_data.random_int_1};
};

TEST_F(OptionCopyConstructorFixture, CopyConstructorWithRandomValue) {
  tmn::Option<int> copied_opt(original_opt);

  ASSERT_TRUE(copied_opt.has_value());
  EXPECT_EQ(copied_opt.value(), test_data.random_int_1);
  ASSERT_TRUE(original_opt.has_value());
  EXPECT_EQ(original_opt.value(), test_data.random_int_1);
}

class OptionAssignmentOperatorsFixture : public ::testing::Test {
protected:
  tmn::test_utils::RandomTestData test_data;
  tmn::Option<int> source_opt{test_data.random_int_1};
  tmn::Option<int> target_opt;
};

TEST_F(OptionAssignmentOperatorsFixture, CopyAssignmentWithRandomValue) {
  target_opt = source_opt;

  ASSERT_TRUE(target_opt.has_value());
  EXPECT_EQ(target_opt.value(), test_data.random_int_1);
  ASSERT_TRUE(source_opt.has_value());
  EXPECT_EQ(source_opt.value(), test_data.random_int_1);
}

TEST_F(OptionAssignmentOperatorsFixture, MoveAssignmentWithRandomValue) {
  const int new_value = tmn::test_utils::generate_random_val(1001, 2000);
  target_opt = tmn::Option<int>(new_value);

  ASSERT_TRUE(target_opt.has_value());
  EXPECT_EQ(target_opt.value(), new_value);
}

class OptionSwapFixture : public ::testing::Test {
protected:
  tmn::test_utils::RandomTestData test_data;
  const int value1 = test_data.random_int_1;
  const int value2 = tmn::test_utils::generate_random_val(1001, 2000);
  tmn::Option<int> opt1{value1};
  tmn::Option<int> opt2{value2};
};

TEST_F(OptionSwapFixture, SwapWithRandomValues) {
  std::swap(opt1, opt2);

  EXPECT_EQ(opt1.value(), value2);
  EXPECT_EQ(opt2.value(), value1);
}

class OptionValueOrFixture : public ::testing::Test {
protected:
  tmn::test_utils::RandomTestData test_data;
  const int default_value = tmn::test_utils::generate_random_val(2001, 3000);
  tmn::Option<int> filled_opt{test_data.random_int_1};
  tmn::Option<int> empty_opt;
};

TEST_F(OptionValueOrFixture, ValueOrWithRandomValues) {
  EXPECT_EQ(filled_opt.value_or(default_value), test_data.random_int_1);
  EXPECT_EQ(empty_opt.value_or(default_value), default_value);
}

TEST_F(OptionValueOrFixture, ValueOrDefaultWithRandomValue) {
  EXPECT_EQ(filled_opt.value_or_default(), test_data.random_int_1);
  EXPECT_EQ(empty_opt.value_or_default(), 0);
}

class OptionValueOrExceptionFixture : public ::testing::Test {
protected:
  tmn::test_utils::RandomTestData test_data;
  tmn::Option<int> filled_opt{test_data.random_int_1};
  tmn::Option<int> empty_opt;
};

TEST_F(OptionValueOrExceptionFixture, ValueAccessWithRandomValue) {
  EXPECT_EQ(filled_opt.value(), test_data.random_int_1);
}

TEST_F(OptionValueOrExceptionFixture, ValueAccessThrowsOnEmpty) {
  EXPECT_THROW(empty_opt.value(), std::bad_optional_access);
}

class OptionDestroyValueFixture : public ::testing::Test {
protected:
  tmn::test_utils::RandomTestData test_data;
  tmn::Option<std::string> string_opt{test_data.random_string};
};

TEST_F(OptionDestroyValueFixture, DestroyValueWithRandomString) {
  EXPECT_TRUE(string_opt.has_value());
  EXPECT_TRUE(string_opt.destroy_value());
  EXPECT_FALSE(string_opt.has_value());
  EXPECT_FALSE(string_opt.destroy_value());
}

class OptionMapFixture : public ::testing::Test {
protected:
  tmn::test_utils::RandomTestData test_data;
  tmn::Option<int> filled_opt{test_data.random_int_1};
  tmn::Option<int> empty_opt;
};

TEST_F(OptionMapFixture, MapWithRandomValue) {
  const auto doubled = filled_opt.fmap([](int x) { return x * 2; });

  ASSERT_TRUE(doubled.has_value());
  EXPECT_EQ(doubled.value(), test_data.random_int_1 * 2);
}

TEST_F(OptionMapFixture, MapWithEmpty) {
  const auto r = empty_opt.fmap([](int x) { return x * 2; });
  EXPECT_FALSE(r.has_value());
}

class OptionAndThenFixture : public ::testing::Test {
protected:
  tmn::test_utils::RandomTestData test_data;
  tmn::Option<int> int_opt{test_data.random_int_1};
  tmn::Option<std::string> string_opt{test_data.random_string};
  tmn::Option<int> empty_opt;
};

TEST_F(OptionAndThenFixture, AndThenWithRandomInt) {
  const auto r = int_opt.and_then([](int x) { return x * 2; });
  ASSERT_TRUE(r.has_value());
  EXPECT_EQ(r.value(), test_data.random_int_1 * 2);
}

TEST_F(OptionAndThenFixture, AndThenWithEmpty) {
  const auto r = empty_opt.and_then([](int x) { return x * 2; });
  EXPECT_FALSE(r.has_value());
}

TEST_F(OptionAndThenFixture, AndThenWithStringLength) {
  const auto r = string_opt.and_then([](const std::string& s) { return s.length(); });
  ASSERT_TRUE(r.has_value());
  EXPECT_EQ(r.value(), test_data.random_string.length());
}

class OptionOrElseFixture : public ::testing::Test {
protected:
  tmn::test_utils::RandomTestData test_data;
  const int fallback_value = tmn::test_utils::generate_random_val(3001, 4000);
  tmn::Option<int> filled_opt{test_data.random_int_1};
  tmn::Option<int> empty_opt;
};

TEST_F(OptionOrElseFixture, OrElseWithRandomValue) {
  bool callback_called = false;
  const auto r = filled_opt.or_else([&]() {
    callback_called = true;
    return fallback_value;
  });

  EXPECT_TRUE(r.has_value());
}

TEST_F(OptionOrElseFixture, OrElseWithEmpty) {
  bool callback_called = false;
  const auto r = empty_opt.or_else([&]() {
    callback_called = true;
    return fallback_value;
  });

  ASSERT_TRUE(r.has_value());
  EXPECT_EQ(r.value(), fallback_value);
  EXPECT_TRUE(callback_called);
}

class OptionChainingFixture : public ::testing::Test {
protected:
  tmn::test_utils::RandomTestData test_data;
  const int fallback_value = tmn::test_utils::generate_random_val(4001, 5000);
};

TEST_F(OptionChainingFixture, AndThenChainWithRandomValue) {
  const auto r = tmn::Option<int>(test_data.random_int_1)
    .and_then([](int x) { return x * 2; })
    .and_then([](int x) { return x + 5; })
    .and_then([](int x) { return std::to_string(x); });

  const int expected_value = (test_data.random_int_1 * 2) + 5;
  ASSERT_TRUE(r.has_value());
  EXPECT_EQ(r.value(), std::to_string(expected_value));
}

TEST_F(OptionChainingFixture, OrElseChain) {
  const auto r = tmn::Option<int>()
    .or_else([this]() { return fallback_value; })
    .or_else([]() { return tmn::test_utils::generate_random_val(5001, 6000); });

  ASSERT_TRUE(r.has_value());
  EXPECT_EQ(r.value(), fallback_value);
}

TEST_F(OptionChainingFixture, MixedChainWithRandomValue) {
  const int initial_value = test_data.random_int_1;
  const auto r = tmn::Option<int>(initial_value)
    .and_then([](int x) { return x % 2 == 0 ? x : x * 2; })
    .or_else([this]() { return fallback_value; })
    .and_then([](int x) { return x * 3; });

  const int transformed_value = (initial_value % 2 == 0) ? initial_value : initial_value * 2;
  const int expected_value = transformed_value * 3;

  ASSERT_TRUE(r.has_value());
  EXPECT_EQ(r.value(), expected_value);
}

class OptionConversionFixture : public ::testing::Test {
protected:
  tmn::test_utils::RandomTestData test_data;
  const std::string error_message = "error_" + std::to_string(test_data.random_int_1);
  tmn::Option<int> filled_opt{test_data.random_int_1};
  tmn::Option<int> empty_opt;
};

TEST_F(OptionConversionFixture, ToResultWithRandomValue) {
  const auto r = filled_opt.to_result(tmn::err::StrErr(error_message));

  ASSERT_TRUE(r.is_ok());
  EXPECT_EQ(r.unwrap_value(), test_data.random_int_1);
}

TEST_F(OptionConversionFixture, ToResultWithEmpty) {
  const auto r = empty_opt.to_result(tmn::err::StrErr(error_message));

  ASSERT_TRUE(r.is_err());
  EXPECT_EQ(r.unwrap_err().err_msg(), error_message);
}
