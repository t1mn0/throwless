#include <cstddef>
#include <gtest/gtest.h>
#include <stdexcept>
#include <string>

#include "../../Result/Result.hpp"
#include "../../Error/Error.hpp"
#include "../../Error/ErrorConcept.hpp"
#include "../RandomGenerator.hpp"

namespace tmn::test_utils {

struct TestErr {
private:
  int code = 0;

public:
  TestErr() = default;
  TestErr(int code) : code(code) {}

  std::string err_msg() const noexcept { return "Error code: " + std::to_string(code); }
  const char* what() const noexcept { return "Unknown"; }

  bool operator==(const TestErr& oth) const noexcept { return code == oth.code; }
};

static_assert(tmn::err::Error<TestErr>, "TestErr must satisfy Error concept");

}

class ResultOkErrConstructorFixture : public ::testing::Test {
protected:
  tmn::test_utils::RandomTestData test_data;

  tmn::err::Result<int, tmn::err::StrErr> ok_result =
    tmn::err::Result<int, tmn::err::StrErr>::Ok(test_data.random_int_1);

  tmn::err::Result<int, tmn::err::StrErr> err_result =
    tmn::err::Result<int, tmn::err::StrErr>::Err(test_data.random_string);
};

TEST_F(ResultOkErrConstructorFixture, OkConstructorWithRandomValue) {
  ASSERT_TRUE(ok_result.is_ok());
  EXPECT_FALSE(ok_result.is_err());
  EXPECT_EQ(ok_result.value(), test_data.random_int_1);
}

TEST_F(ResultOkErrConstructorFixture, ErrConstructorWithRandomValue) {
  ASSERT_TRUE(err_result.is_err());
  EXPECT_FALSE(err_result.is_ok());
  EXPECT_EQ(err_result.err(), test_data.random_string);
}

class ResultCopyConstructorFixture : public ::testing::Test {
protected:
  tmn::test_utils::RandomTestData test_data;
  tmn::err::Result<int, tmn::err::StrErr> original_ok =
    tmn::err::Result<int, tmn::err::StrErr>::Ok(test_data.random_int_1);

  tmn::err::Result<int, tmn::err::StrErr> original_err =
    tmn::err::Result<int, tmn::err::StrErr>::Err(test_data.random_string);
};

TEST_F(ResultCopyConstructorFixture, CopyOkResult) {
  tmn::err::Result<int, tmn::err::StrErr> copied = original_ok;

  ASSERT_TRUE(copied.is_ok());
  EXPECT_EQ(copied.value(), test_data.random_int_1);
  ASSERT_TRUE(original_ok.is_ok());
  EXPECT_EQ(original_ok.value(), test_data.random_int_1);
}

TEST_F(ResultCopyConstructorFixture, CopyErrResult) {
  tmn::err::Result<int, tmn::err::StrErr> copied = original_err;

  ASSERT_TRUE(copied.is_err());
  EXPECT_EQ(copied.err(), test_data.random_string);
  ASSERT_TRUE(original_err.is_err());
  EXPECT_EQ(original_err.err(), test_data.random_string);
}

class ResultMoveConstructorFixture : public ::testing::Test {
protected:
  tmn::test_utils::RandomTestData test_data;
};

TEST_F(ResultMoveConstructorFixture, MoveOkResult) {
  tmn::err::Result<std::string, tmn::test_utils::TestErr> original =
    tmn::err::Result<std::string, tmn::test_utils::TestErr>::Ok(test_data.random_string);

  tmn::err::Result<std::string, tmn::test_utils::TestErr> moved = std::move(original);

  ASSERT_TRUE(moved.is_ok());
  EXPECT_EQ(moved.value(), test_data.random_string);
}

TEST_F(ResultMoveConstructorFixture, MoveErrResult) {
  tmn::err::Result<int, tmn::err::StrErr> original =
    tmn::err::Result<int, tmn::err::StrErr>::Err(test_data.random_string);

  tmn::err::Result<int, tmn::err::StrErr> moved = std::move(original);

  ASSERT_TRUE(moved.is_err());
  EXPECT_EQ(moved.err(), test_data.random_string);
}

class ResultAssignmentFixture : public ::testing::Test {
protected:
  tmn::test_utils::RandomTestData test_data;
  tmn::err::Result<int, tmn::err::StrErr> target;

  // Since Result() = delete, `target` initialized manually:
  ResultAssignmentFixture()
    : target(tmn::err::Result<int, tmn::err::StrErr>::Err("initial")) {}
};

TEST_F(ResultAssignmentFixture, CopyAssignmentOk) {
  tmn::err::Result<int, tmn::err::StrErr> source =
    tmn::err::Result<int, tmn::err::StrErr>::Ok(test_data.random_int_1);

  target = source;

  ASSERT_TRUE(target.is_ok());
  EXPECT_EQ(target.value(), test_data.random_int_1);
  ASSERT_TRUE(source.is_ok());
  EXPECT_EQ(source.value(), test_data.random_int_1);
}

TEST_F(ResultAssignmentFixture, MoveAssignmentErr) {
  tmn::err::Result<int, tmn::err::StrErr> source =
    tmn::err::Result<int, tmn::err::StrErr>::Err(test_data.random_string);

  target = std::move(source);

  ASSERT_TRUE(target.is_err());
  EXPECT_EQ(target.err(), test_data.random_string);
}

class ResultBoolConversionFixture : public ::testing::Test {
protected:
  tmn::test_utils::RandomTestData test_data;
};

TEST_F(ResultBoolConversionFixture, BoolConversionOk) {
  tmn::err::Result<int, tmn::err::StrErr> ok_result =
    tmn::err::Result<int, tmn::err::StrErr>::Ok(test_data.random_int_1);

  EXPECT_TRUE(static_cast<bool>(ok_result));
}

TEST_F(ResultBoolConversionFixture, BoolConversionErr) {
  tmn::err::Result<int, tmn::err::StrErr> err_result =
      tmn::err::Result<int, tmn::err::StrErr>::Err(test_data.random_string);

  EXPECT_FALSE(static_cast<bool>(err_result));
}

class ResultToOptionFixture : public ::testing::Test {
protected:
  tmn::test_utils::RandomTestData test_data;
};

TEST_F(ResultToOptionFixture, ToOptionFromOk) {
  tmn::err::Result<int, tmn::err::StrErr> ok_result =
      tmn::err::Result<int, tmn::err::StrErr>::Ok(test_data.random_int_1);

  auto option = ok_result.to_option();

  ASSERT_TRUE(option.has_value());
  EXPECT_EQ(option.value(), test_data.random_int_1);
}

TEST_F(ResultToOptionFixture, ToOptionFromErr) {
  tmn::err::Result<int, tmn::err::StrErr> err_result =
    tmn::err::Result<int, tmn::err::StrErr>::Err(test_data.random_string);

  auto option = err_result.to_option();

  EXPECT_FALSE(option.has_value());
}

class ResultValueAccessFixture : public ::testing::Test {
protected:
  tmn::test_utils::RandomTestData test_data;
  tmn::err::Result<int, tmn::err::StrErr> ok_result =
    tmn::err::Result<int, tmn::err::StrErr>::Ok(test_data.random_int_1);

  tmn::err::Result<int, tmn::err::StrErr> err_result =
    tmn::err::Result<int, tmn::err::StrErr>::Err(test_data.random_string);
};

TEST_F(ResultValueAccessFixture, ValueAccessOk) {
  EXPECT_EQ(ok_result.value(), test_data.random_int_1);
  EXPECT_EQ(ok_result.value_or_default(), test_data.random_int_1);
}

TEST_F(ResultValueAccessFixture, ValueAccessErrThrows) {
  EXPECT_THROW(err_result.value(), std::runtime_error);
}

TEST_F(ResultValueAccessFixture, ValueOrWithOk) {
  const int default_val = tmn::test_utils::generate_random_val(1001, 2000);
  EXPECT_EQ(ok_result.value_or(default_val), test_data.random_int_1);
}

TEST_F(ResultValueAccessFixture, ValueOrWithErr) {
  const int default_val = tmn::test_utils::generate_random_val(1001, 2000);
  EXPECT_EQ(err_result.value_or(default_val), default_val);
}

class ResultErrAccessFixture : public ::testing::Test {
protected:
  tmn::test_utils::RandomTestData test_data;
  tmn::err::Result<int, tmn::err::StrErr> ok_result =
    tmn::err::Result<int, tmn::err::StrErr>::Ok(test_data.random_int_1);

  tmn::err::Result<int, tmn::err::StrErr> err_result =
    tmn::err::Result<int, tmn::err::StrErr>::Err(test_data.random_string);
};

TEST_F(ResultErrAccessFixture, ErrAccessOkThrows) {
  EXPECT_THROW(ok_result.err(), std::runtime_error);
}

TEST_F(ResultErrAccessFixture, ErrAccessErr) {
  EXPECT_EQ(err_result.err(), test_data.random_string);
}

TEST_F(ResultErrAccessFixture, OptionalErrOk) {
  auto err_opt = ok_result.optional_err();
  EXPECT_FALSE(err_opt.has_value());
}

TEST_F(ResultErrAccessFixture, OptionalErrErr) {
  auto err_opt = err_result.optional_err();
  ASSERT_TRUE(err_opt.has_value());
  EXPECT_EQ(err_opt.value(), test_data.random_string);
}

class ResultFmapFixture : public ::testing::Test {
protected:
  tmn::test_utils::RandomTestData test_data;
};

TEST_F(ResultFmapFixture, FmapOk) {
  tmn::err::Result<int, tmn::err::StrErr> ok_result =
    tmn::err::Result<int, tmn::err::StrErr>::Ok(test_data.random_int_1);

  auto transformed = ok_result.fmap([](int x) { return x * 2; });

  ASSERT_TRUE(transformed.is_ok());
  EXPECT_EQ(transformed.value(), test_data.random_int_1 * 2);
}

TEST_F(ResultFmapFixture, FmapErr) {
  tmn::err::Result<int, tmn::err::StrErr> err_result =
    tmn::err::Result<int, tmn::err::StrErr>::Err(test_data.random_string);

  auto transformed = err_result.fmap([](int x) { return x * 2; });

  ASSERT_TRUE(transformed.is_err());
  EXPECT_EQ(transformed.err(), test_data.random_string);
}

class ResultAndThenFixture : public ::testing::Test {
protected:
  tmn::test_utils::RandomTestData test_data;
};

TEST_F(ResultAndThenFixture, AndThenOk) {
  tmn::err::Result<int, tmn::err::StrErr> ok_result =
    tmn::err::Result<int, tmn::err::StrErr>::Ok(test_data.random_int_1);

  auto result = ok_result.and_then([](int x) {
    return std::to_string(x);
  });

  ASSERT_TRUE(result.is_ok());
  EXPECT_EQ(result.value(), std::to_string(test_data.random_int_1));
}

TEST_F(ResultAndThenFixture, AndThenErr) {
  tmn::err::Result<int, tmn::err::StrErr> err_result =
    tmn::err::Result<int, tmn::err::StrErr>::Err(test_data.random_string);

  auto result = err_result.and_then([](int x) {
    return tmn::err::Result<std::string, tmn::err::StrErr>::Ok(std::to_string(x));
  });

  ASSERT_TRUE(result.is_err());
  EXPECT_EQ(result.err(), test_data.random_string);
}

class ResultTemplateMethodsFixture : public ::testing::Test {
protected:
  tmn::test_utils::RandomTestData test_data;
};

TEST_F(ResultTemplateMethodsFixture, OkWithArgs) {
  auto result = tmn::err::Result<std::string, tmn::test_utils::TestErr>::Ok("test", std::size_t{3});

  ASSERT_TRUE(result.is_ok());
  EXPECT_EQ(result.value(), "tes");
}

TEST_F(ResultTemplateMethodsFixture, ErrWithArgs) {
  auto result = tmn::err::Result<int, tmn::err::StrErr>::Err("error", std::size_t{4});

  ASSERT_TRUE(result.is_err());
  EXPECT_EQ(result.err(), "erro");
}

class ResultSwapFixture : public ::testing::Test {
protected:
  tmn::test_utils::RandomTestData test_data;
};

TEST_F(ResultSwapFixture, SwapResults) {
  tmn::err::Result<int, tmn::err::StrErr> result1 =
    tmn::err::Result<int, tmn::err::StrErr>::Ok(test_data.random_int_1);

  tmn::err::Result<int, tmn::err::StrErr> result2 =
    tmn::err::Result<int, tmn::err::StrErr>::Err(test_data.random_string);

  std::swap(result1, result2);

  ASSERT_TRUE(result1.is_err());
  EXPECT_EQ(result1.err(), test_data.random_string);

  ASSERT_TRUE(result2.is_ok());
  EXPECT_EQ(result2.value(), test_data.random_int_1);
}
