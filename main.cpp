#include "Option/Option.hpp"
#include "Error/Error.hpp"
#include <iostream>

using namespace tmn::err;

int main(){
  Result<int, StrErr> r = Result<int, StrErr>::Err("strochka");
  std::string str = "string";
  Option<std::string> opt = Some(str);
  std::cout << r.is_err() << std::endl;
}
