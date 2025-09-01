#include <gtest/gtest.h>

#include <stdexcept>
#include <string>

#include "../../Error/Error.hpp"

TEST(ExceptionErrTest, BasicFunctionality) {
  struct TestException : std::exception {
    const char* what() const noexcept override { return "Test exception"; }
  };

  TestException ex;
  tmn::err::ExceptionErr excerr1(ex);

  EXPECT_TRUE(excerr1.err_msg().find("TestException") != std::string::npos);
  EXPECT_TRUE(excerr1.err_msg().find("Test exception") != std::string::npos);

  tmn::err::ExceptionErr err2;
  EXPECT_EQ(err2.err_msg(), "[Unknown type]: Unknown error");

  TestException excerr2;
  tmn::err::ExceptionErr excerr3(excerr2);
  EXPECT_EQ(excerr1, excerr3);
  EXPECT_NE(excerr1, excerr2);
}

TEST(TryOrConvertTest, BasicFunctionality) {
  auto success_fn = []() -> int { return 42; };
  auto success_result = tmn::err::try_or_convert(success_fn);
  EXPECT_TRUE(success_result.is_ok());
  EXPECT_EQ(success_result.unwrap_val(), 42);

  auto throwing_fn = []() -> int { throw std::runtime_error("Test error"); };
  auto error_result = tmn::err::try_or_convert(throwing_fn);
  EXPECT_TRUE(error_result.is_err());
  EXPECT_TRUE(error_result.unwrap_err().err_msg().find("runtime_error") != std::string::npos);
  EXPECT_TRUE(error_result.unwrap_err().err_msg().find("Test error") != std::string::npos);

  auto unknown_throwing_fn = []() -> int { throw 42; };
  auto unknown_error_result = tmn::err::try_or_convert(unknown_throwing_fn);
  EXPECT_TRUE(unknown_error_result.is_err());
  EXPECT_EQ(unknown_error_result.unwrap_err().err_msg(), "[unknown]: Unknown exception");
}
