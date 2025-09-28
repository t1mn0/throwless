#include <iostream>
#include <string>

#include "Option/Option.hpp"
#include "SmartPtr/UniquePtr/UniquePtr.hpp"
#include "SmartPtr/SharedPtr/SharedPtr.hpp"

tmn::Option<std::string> find_user_name(int user_id) {
  if (user_id == 1) return std::string("Alice");
  if (user_id == 2) return std::string("Bob");
  return tmn::None<std::string>();
}

int main() {
  auto name = find_user_name(3)
    .fmap([](auto s) { return "Hello, " + s + "!"; })
    .or_else([&]() {
      return std::string("User not found");
    });

  if (name.has_value()){
    std::string* name_on_heap = new std::string(name.value());
    tmn::UniquePtr<std::string> up(name_on_heap);
    // some operations with unique pointer;
    // ...
    std::string* backup_on_heap = new std::string("Object wasnt constructed");
    std::cout << *up.try_get().value_or(backup_on_heap) << std::endl; // cout: User not found;
  }

  // - - - - - - - - - - - - - - - - - - - - - - -
  tmn::SharedPtr<std::string[]> ptr1(new std::string[2]{"qwerty", "dvorak"}, 2);
  tmn::SharedPtr<std::string[]> ptr2 = ptr1;
  std::cout << ptr1.use_count() << std::endl; // cout: 2;

  return 0;
}
