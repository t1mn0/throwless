#include <gtest/gtest.h>

#include <string>

#include "../../Option/Option.hpp"
#include "../_TestUtils/RandomGenerator.hpp"

class OptionArithmeticTestFixture : public ::testing::Test {
protected: //* fields:
  tmn::test_utils::RandomTestData test_data;

protected: //* methods:
  void SetUp() override {
    test_data = tmn::test_utils::RandomTestData{};
  }

};

TEST_F(OptionArithmeticTestFixture, IntAdditionWithRandomValues) {
  tmn::Option<int> a(test_data.random_int_1);
  tmn::Option<int> b(test_data.random_int_2);

  const auto c = a + b;

  ASSERT_TRUE(c.has_value()) << "Sum of the Option<int> for two Options with valid values should have the value";
  EXPECT_EQ(c.value(), test_data.random_int_1 + test_data.random_int_2);
}

TEST_F(OptionArithmeticTestFixture, DoubleAdditionWithRandomValues) {
  tmn::Option<double> a(test_data.random_double_1);
  tmn::Option<double> b(test_data.random_double_2);

  const auto c = a + b;

  ASSERT_TRUE(c.has_value()) << "Sum of the Option<double> for two Options with valid values should have the value";
  EXPECT_DOUBLE_EQ(c.value(), test_data.random_double_1 + test_data.random_double_2);
}

TEST_F(OptionArithmeticTestFixture, SubtractionWithRandomValues) {
  tmn::Option<int> a(test_data.random_int_1);
  tmn::Option<int> b(test_data.random_int_2);

  const auto c = a - b;

  ASSERT_TRUE(c.has_value()) << "Subtraction of the Option<int> for two Options with valid values should have the value";
  EXPECT_EQ(c.value(), test_data.random_int_1 - test_data.random_int_2);
}

TEST_F(OptionArithmeticTestFixture, MultiplicationWithRandomValues) {
  tmn::Option<int> a(test_data.random_int_1);
  tmn::Option<int> b(test_data.random_int_2);

  const auto c = a * b;

  ASSERT_TRUE(c.has_value()) << "Multiplication of the Option<int> for two Options with valid values should have the value";
  EXPECT_EQ(c.value(), test_data.random_int_1 * test_data.random_int_2);
}

TEST_F(OptionArithmeticTestFixture, DivisionWithRandomValues) {
  // Divisor must not be zero:
  const int divisor = tmn::test_utils::generate_random_val(
    1,
    tmn::test_utils::RandomTestData::max_int
  );

  tmn::Option<int> dividend(test_data.random_int_1);
  tmn::Option<int> divisor_opt(divisor);

  const auto c = dividend / divisor_opt;

  ASSERT_TRUE(c.has_value()) << "Division of the Option<int> for two Options with valid values should have the value";
  EXPECT_EQ(c.value(), test_data.random_int_1 / divisor);
}

TEST_F(OptionArithmeticTestFixture, DivisionByZero) {
  tmn::Option<int> a(test_data.random_int_1);
  tmn::Option<int> zero(0);

  const auto c = a / zero;

  EXPECT_FALSE(c.has_value());
}

TEST_F(OptionArithmeticTestFixture, CompoundAssignmentOperators) {
  tmn::Option<int> a(test_data.random_int_1);
  // Divisor must not be zero:
  const tmn::Option<int> b = tmn::test_utils::generate_random_val(
    1,
    tmn::test_utils::RandomTestData::max_int
  );

  const int original_value = a.value();

  a += b;
  ASSERT_TRUE(a.has_value()) << "CompoundAssignmentSum of the Option<int> for two Options with valid values should have the value";
  EXPECT_EQ(a.value(), original_value + b.value());

  a -= b;
  ASSERT_TRUE(a.has_value()) << "CompoundAssignmentSubtraction of the Option<int> for two Options with valid values should have the value";
  EXPECT_EQ(a.value(), original_value);

  a *= b;
  ASSERT_TRUE(a.has_value()) << "CompoundAssignmentMultiplication of the Option<int> for two Options with valid values should have the value";
  EXPECT_EQ(a.value(), original_value * b.value());

  a = tmn::Option<int>(original_value * b.value());
  a /= b;
  ASSERT_TRUE(a.has_value()) << "CompoundAssignmentDivision of the Option<int> for two Options with valid values should have the value";
  EXPECT_EQ(a.value(), original_value);
}

TEST_F(OptionArithmeticTestFixture, MixedTypesArithmetic) {
  tmn::Option<int> int_val(test_data.random_int_1);
  tmn::Option<double> double_val(test_data.random_double_1);

  const auto res = int_val * double_val;

  static_assert(std::is_same_v<decltype(res), const tmn::Option<double>>);
  ASSERT_TRUE(res.has_value()) << "MixedTypesMultiplication of the Option(int&double) for two Options with valid values should have the value";
  EXPECT_DOUBLE_EQ(res.value(), test_data.random_int_1 * test_data.random_double_1);
}

TEST_F(OptionArithmeticTestFixture, StringConcatenation) {
  const std::string prefix = "Hello";
  const std::string suffix = " World";

  tmn::Option<std::string> a(prefix);
  tmn::Option<std::string> b(suffix);

  const auto c = a + b;

  ASSERT_TRUE(c.has_value()) << "After concatenating valid strings under Option<string>, we should get Option<string> with a valid value";
  EXPECT_EQ(c.value(), prefix + suffix);
}

TEST_F(OptionArithmeticTestFixture, OperationsWithEmptyOperands) {
  tmn::Option<int> filled_value(test_data.random_int_1);
  tmn::Option<int> empty_value;

  // Binary operations with empty operand:
  const auto addition_result = filled_value + empty_value;
  const auto subtraction_result = empty_value - filled_value;

  EXPECT_FALSE(addition_result.has_value());
  EXPECT_FALSE(subtraction_result.has_value());

  // CompoundAssignment operations with empty operand:
  tmn::Option<int> mutable_value(test_data.random_int_1);
  mutable_value += empty_value;

  EXPECT_FALSE(mutable_value.has_value());
}

TEST_F(OptionArithmeticTestFixture, ChainedOperations) {
  const int value1 = test_data.random_int_1;
  const int value2 = test_data.random_int_2;
  const int value3 = tmn::test_utils::generate_random_val(
    tmn::test_utils::RandomTestData::min_int,
    tmn::test_utils::RandomTestData::max_int);

  tmn::Option<int> a(value1);
  tmn::Option<int> b(value2);
  tmn::Option<int> c(value3);

  const auto d = (a + b) * c;

  ASSERT_TRUE(d.has_value());
  EXPECT_EQ(d.value(), (value1 + value2) * value3);
}
