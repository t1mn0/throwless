#include "Result/Result.hpp"
#include "Error/Error.hpp"
#include <iostream>

int main(){
  tmn::err::Result<int, tmn::err::StrErr> r = tmn::err::StrErr("strochka");
  std::cout << r.is_err() << std::endl;
}
