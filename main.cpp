#include <iostream>

#include "include/Error/Error.hpp"
#include "include/Error/TryOrConvert.hpp"
#include "include/Option/Option.hpp"
#include "include/Result/Result.hpp"

using namespace tmn;

// You can determine the type of your Errors by yourself,
// depending on the specifics of the your project:
using FailedHostExtracting = err::StrErr;
using FailedPortExtracting = err::StrErr;
using FailedTimeoutExtracting = err::StrErr;

// Just stopper:
Result<std::string, FailedHostExtracting> extract_host(const std::string& json_str){
  return Result<std::string, FailedHostExtracting>::Ok("webserver.example.com");
}

Result<int, FailedPortExtracting> extract_port(const std::string& json_str){
  return Result<int, FailedPortExtracting>::Ok(8080);
  // return Result<int, FailedPortExtracting>::Err("Bad port");
}

Result<int, FailedTimeoutExtracting> extract_timeout(const std::string& json_str){
  // return Result<int, FailedTimeoutExtracting>::Ok(1200);
   return Result<int, FailedTimeoutExtracting>::Err("Bad timeout");
}

struct Config {
  std::string host;
  int port;
  tmn::Option<int> timeout_ms;
};

tmn::Result<Config, err::AnyErr> parse_config(const std::string& json_str) {
  auto host_result = extract_host(json_str);
  auto port_result = extract_port(json_str);

  auto func = [&]() -> Config {
    return Config {
      .host = host_result.unwrap_value(),
      .port = port_result.unwrap_value(),
      .timeout_ms = extract_timeout(json_str).to_option()
    };
  };

  return tmn::try_or_convert(func);
}

int main() {
  auto config = parse_config(R"({"host": "localhost", "port": 8080})");

  if (config.is_ok()) {
    std::cout << "Host: " << config.unwrap_value().host << std::endl;
    std::cout << "Port: " << config.unwrap_value().port << std::endl;
    std::cout << "Timeout: ";
    std::cout << config.unwrap_value().timeout_ms.value_or(-1) << std::endl;
  }
  else {
    std::cerr << "Config error: " << config.unwrap_err().err_msg() << std::endl;
  }

  return 0;
}
