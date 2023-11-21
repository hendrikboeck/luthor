#pragma once

#include <fmt/core.h>

#include <stdexcept>
#include <string>

namespace luthor {

class error : public std::runtime_error {
public:
  error(const std::string message);
};

class not_implemented_error : public error {
public:
  not_implemented_error();
};

class key_not_found_error : public error {
public:
  template<typename T>
  inline key_not_found_error(const T key)
      : error(
        fmt::format("key '{}' not found, but is required", std::string(key))
      ) { }
};

class wrong_type_error : public error {
public:
  template<typename T>
  inline wrong_type_error(const T correct_type)
      : error(fmt::format("object has to be type {}", std::string(correct_type))
      ) { }
};

}  // namespace luthor
