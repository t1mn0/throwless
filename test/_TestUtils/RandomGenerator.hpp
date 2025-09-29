#pragma once
#ifndef TMN_THROWLESS_RANDOMGENERATOR_HPP
#define TMN_THROWLESS_RANDOMGENERATOR_HPP

#include <random>
#include <string>
#include <type_traits>

namespace tmn::test_utils {

inline std::mt19937& get_random_engine() {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  return gen;
}

template<typename T>
T generate_random_val(T min, T max) {
  if constexpr (std::is_integral_v<T>) {
    std::uniform_int_distribution<T> dist(min, max);
    return dist(get_random_engine());
  } else {
    std::uniform_real_distribution<T> dist(min, max);
    return dist(get_random_engine());
  }
}

struct RandomTestData {
  static constexpr int min_int = 1;
  static constexpr int max_int = 100;
  static constexpr double min_double = 0.1;
  static constexpr double max_double = 10.0;

  int random_int_1;
  int random_int_2;
  double random_double_1;
  double random_double_2;
  std::string random_string;

  RandomTestData() :
    random_int_1(generate_random_val(min_int, max_int)),
    random_int_2(generate_random_val(min_int, max_int)),
    random_double_1(generate_random_val(min_double, max_double)),
    random_double_2(generate_random_val(min_double, max_double)),
    random_string("test_" + std::to_string(random_int_1)) {}
};

} // namespace tmn::test_utils;

#endif
