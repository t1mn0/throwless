#include <gtest/gtest.h>
#include "../../include/Error/Error.hpp"

using namespace throwless;

TEST(ErrorConceptTest, ValidErrorTypes) {
    static_assert(Error<StringError>);
    static_assert(Error<CodeError>);
    static_assert(Error<ExceptionError>);
}

TEST(ErrorConceptTest, InvalidErrorTypes) {
    static_assert(!Error<int>);
    static_assert(!Error<std::string>);
}

TEST(StringErrorTest, BasicFunctionality) {
    StringError err;
    EXPECT_EQ(err.message(), "Undefined error");
    
    StringError custom{"File not found"};
    EXPECT_EQ(custom.message(), "File not found");
}

TEST(CodeErrorTest, BasicFunctionality) {
    CodeError err{404};
    EXPECT_EQ(err.message(), "Error code: 404");
}

TEST(ExceptionErrorTest, Construction) {
    try {
        throw std::runtime_error("Test error");
    } catch (...) {
        ExceptionError err{std::current_exception(), std::string("ErrContext")};
        // TODO : consider how an exception message can be passed to the ExceptinoError type
        #if 0 
        EXPECT_TRUE(err.message().find("Test error") != std::string::npos);
        #endif
        EXPECT_TRUE(err.message().find("ErrContext") != std::string::npos);
    }
    
    ExceptionError empty;
    EXPECT_EQ(empty.message(), "ExceptionError with context: 'empty Context'");
}

TEST(CaptureExceptionTest, SuccessCase) {
    auto res = capture_exception([](int x) { return x * 2; }, 21);
    ASSERT_TRUE(res.is_ok());
    EXPECT_EQ(res.unwrap_or(-1), 42);
}

TEST(CaptureExceptionTest, ExceptionCase) {
    auto res = capture_exception([] { throw std::runtime_error("Error"); });
    ASSERT_TRUE(res.is_err());
    // TODO :
    #if 0 
    EXPECT_TRUE(res.unwrap_err_or_default().message().find("Error") != std::string::npos);
    #endif
}

TEST(TryOrConvertTest, SuccessCase) {
    auto res = try_or_convert([](int a, int b) { return a + b; }, 40, 2);
    ASSERT_TRUE(res.is_ok());
    EXPECT_EQ(res.unwrap_or(-1), 42);
}

TEST(TryOrConvertTest, ExceptionConversion) {
    auto res = try_or_convert([] { 
        throw std::invalid_argument("Bad input"); 
    });
    
    ASSERT_TRUE(res.is_err());
    // TODO :
    #if 0 
    EXPECT_TRUE(res.unwrap_err_or_default().message().find("Bad input") != std::string::npos);
    #endif
}

TEST(ErrorResultIntegrationTest, ResultWithStringError) {
    Result<int, StringError> res = Result<int, StringError>::Err(std::move(StringError{"Failed"}));
    ASSERT_TRUE(res.is_err());
    // TODO :
    #if 0 
    EXPECT_EQ(res.unwrap_err_or_default().message(), "Failed");
    #endif
}

TEST(ErrorResultIntegrationTest, VoidResultWithExceptionError) {
    Result<void, ExceptionError> res = Result<void, ExceptionError>::Err(std::move(ExceptionError{"ErrContext"}));
    ASSERT_TRUE(res.is_err());
    // TODO :
    #if 0 
    EXPECT_TRUE(res.unwrap_err_or_default().message().find("ErrContext") != std::string::npos);
    #endif
}