#include <gtest/gtest.h>

#include <stdexcept>
#include <string>

#include "../../include/Error/TryOrConvert.hpp"
#include "../../include/Error/Error.hpp"

TEST(TryOrConvertTest, BasicFunctionality) {
  auto success_fn = [](int x) -> int { return x*42; };
  auto success_result = tmn::try_or_convert(success_fn, 2);
  EXPECT_TRUE(success_result.is_ok());
  EXPECT_EQ(success_result.unwrap_value(), 84);

  auto throwing_fn = []() -> int { throw std::runtime_error("Test error"); };
  auto error_result = tmn::try_or_convert(throwing_fn);
  EXPECT_TRUE(error_result.is_err());
  EXPECT_TRUE(error_result.unwrap_err().err_msg().find("runtime_error") != std::string::npos);
  EXPECT_TRUE(error_result.unwrap_err().err_msg().find("Test error") != std::string::npos);

  auto unknown_throwing_fn = []() -> int { throw 42; };
  auto unknown_error_result = tmn::try_or_convert(unknown_throwing_fn);
  EXPECT_TRUE(unknown_error_result.is_err());
  EXPECT_EQ(unknown_error_result.unwrap_err().err_msg(), "[Unknown type]: Unknown General Exception Error");
}
