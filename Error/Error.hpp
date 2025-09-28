#ifndef TMN_THROWLESS_ERROR_HPP
#define TMN_THROWLESS_ERROR_HPP

#include <string>
#include <concepts> // for: convertible_to;
#include <typeinfo> // for: typeid;
#include <limits> // for: std::numeric_limits;

#include "ErrorConcept.hpp"

namespace tmn::err {

//* <--- Basic wrappers for classic error handling methods --->

struct AnyErr {
protected:
  std::string msg_;

public:
  AnyErr() = default;
  AnyErr(const std::string& msg) : msg_(msg) {}
  AnyErr(const char* msg) : msg_(msg) {}

  template <typename...Args> requires std::constructible_from<std::string, Args...>
  AnyErr(Args&&... args) : msg_(args...) {}

  virtual ~AnyErr() = default;

  virtual std::string err_msg() const { return msg_; }
  virtual const char* what() const noexcept { return msg_.c_str(); }

  virtual bool operator==(const AnyErr& oth) const noexcept {
    return msg_ == oth.msg_;
  }
};

struct StrErr : public AnyErr {
public:
  // constructors are not declared `explicit` to simplify
  // the construction of objects in complex structures (as Result<T, E>);
  // Compare the options:
  // 1. return Result<Config, StrErr>::Err(StrErr("Port must be in range 1-65535"));
  // 2. return Result<Config, StrErr>::Err("Port must be in range 1-65535");
  // Second option is more laconic and does not contain unnecessary syntactic repetitions Err-Err;
  // Similar reasoning is applied to other wrappers of Error<E>;
  StrErr() : AnyErr("Undefined error") {}
  StrErr(const std::string& msg) : AnyErr(msg) {}
  StrErr(const char* msg) : AnyErr(msg) {}

  template <typename...Args> requires std::constructible_from<std::string, Args...>
  StrErr(Args&&... args) : AnyErr(args...) {}

  bool operator==(const StrErr& oth) const noexcept {
    const auto* derived = dynamic_cast<const StrErr*>(&oth);
    return derived->msg_ == oth.msg_;
  }
};

// Check that StrErr satisfies the concept of Error<E>:
static_assert(Error<StrErr>, "StrErr must satisfy Error concept");

struct GeneralExceptionErr : public AnyErr {
private:
  std::string _exception_name = "Unknown type";

public:
  template<typename E> requires std::derived_from<E, std::exception>
  explicit GeneralExceptionErr(const E& ex) : AnyErr(ex.what()), _exception_name(typeid(ex).name()) {}
  GeneralExceptionErr() : AnyErr("Unknown error") {}

  std::string exception_name() const noexcept { return _exception_name; }
  std::string err_msg() const noexcept { return "[" + _exception_name + "]: "+ msg_; }
  const char* what() const noexcept { return msg_.c_str(); }

  bool operator==(const GeneralExceptionErr& oth) const noexcept {
    return msg_ == oth.msg_ && _exception_name == oth._exception_name;
  }
};

// Check that StrErr satisfies the concept of Error<E>:
static_assert(Error<GeneralExceptionErr>, "ExceptionErr must satisfy Error concept");

struct OutOfRangeErr : public AnyErr {
private:
  std::size_t index_ = std::numeric_limits<std::size_t>::max();
  std::size_t size_ = 0;

public:
  OutOfRangeErr(std::size_t index, std::size_t size, bool is_zero_indexing = true)
    : index_(index), size_(size)
  {
    msg_ = "Index " + std::to_string(index) + " out of range in [0, " + std::to_string(size);
    if (is_zero_indexing) { msg_.push_back(')'); }
    else { msg_.push_back(']'); }
  }

  std::size_t index() const noexcept { return index_; }
  std::size_t size() const noexcept { return size_; }

  std::string err_msg() const { return msg_; }

  const char* what() const noexcept { return msg_.c_str(); }

  bool operator==(const OutOfRangeErr& oth) const noexcept {
    return index_ == oth.index_ && size_ == oth.size_;
  }
};

// Check that OutOfRangeErr satisfies the concept of Error<E>:
static_assert(Error<OutOfRangeErr>, "OutOfRangeErr must satisfy Error concept");

class EmptyArrErr : public AnyErr {
public:
  EmptyArrErr() {}
  EmptyArrErr(std::string msg) : AnyErr(msg) {}

  std::string err_msg() const { return msg_; }

  const char* what() const noexcept { return msg_.c_str(); }

  bool operator==(const EmptyArrErr& oth) const noexcept {
    return msg_ == oth.msg_;
  }
};

static_assert(Error<EmptyArrErr>, "EmptyArrErr must satisfy Error concept");

} // namespace tmn::err;

#endif // TMN_THROWLESS_ERROR_HPP;
