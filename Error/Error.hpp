#ifndef THROWLESS_ERROR_HPP
#define THROWLESS_ERROR_HPP

#include <string>
#include <concepts> // for: convertible_to;
#include <typeinfo> // for: typeid;

#include "ErrorConcept.hpp"

namespace tmn::err {

//* <--- Basic wrappers for classic error handling methods --->

struct StrErr {
private:
  std::string msg = "Undefined error";

public:
  // constructors are not declared `explicit` to simplify
  // the construction of objects in complex structures (as Result<T, E>);
  // Compare the options:
  // 1. return Result<Config, StrErr>::Err(StrErr("Port must be in range 1-65535"));
  // 2. return Result<Config, StrErr>::Err("Port must be in range 1-65535");
  // Second option is more laconic and does not contain unnecessary syntactic repetitions Err-Err;
  // Similar reasoning is applied to other wrappers of Error<E>;
  StrErr(const std::string& msg) : msg(msg) {}
  StrErr(const char* msg) : msg(msg) {}

  template <typename...Args> requires std::constructible_from<std::string, Args...>
  StrErr(Args&&... args) : msg(args...) {}

  std::string err_msg() const noexcept { return msg; }
  const char* what() const noexcept { return msg.c_str(); }

  bool operator==(const StrErr& oth) const noexcept { return msg == oth.msg; }
};

// Check that StrErr satisfies the concept of Error<E>:
static_assert(Error<StrErr>, "StrErr must satisfy Error concept");

struct ExceptionErr {
private:
  std::string msg = "Unknown error";
  std::string _exception_name = "Unknown type";

public:
  template<typename E> requires std::derived_from<E, std::exception>
  explicit ExceptionErr(const E& ex) : msg(ex.what()), _exception_name(typeid(ex).name()) {}
  ExceptionErr() = default;

  std::string exception_name() const noexcept { return _exception_name; }
  std::string err_msg() const noexcept { return "[" + _exception_name + "]: "+ msg; }
  const char* what() const noexcept { return msg.c_str(); }

  bool operator==(const ExceptionErr& oth) const noexcept { return _exception_name == oth._exception_name && msg == oth.msg; }
};

// Check that StrErr satisfies the concept of Error<E>:
static_assert(Error<ExceptionErr>, "ExceptionErr must satisfy Error concept");

} // namespace 'tmn::err';

#endif // THROWLESS_ERROR_HPP;
