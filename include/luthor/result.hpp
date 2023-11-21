#pragma once

// #include <boost/variant2/variant.hpp>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <variant>

#include "luthor/_comptime.hpp"
#include "luthor/error.hpp"

namespace luthor {

// using _result_base = boost::variant2::variant<nlohmann::json*, error>;
using _result_base = std::variant<nlohmann::json*, error>;

constexpr const std::size_t RESULT_OK_INDEX = 0;
constexpr const std::size_t RESULT_ERR_INDEX = 1;

class result : public _result_base {
public:
  using json = nlohmann::json;
  using _result_base::variant;

  result(json& data) noexcept : _result_base(&data) { }

  ~result() noexcept = default;

  [[nodiscard]]
  constexpr bool is_ok() const noexcept {
    return index() == RESULT_OK_INDEX;
  }

  [[nodiscard]]
  constexpr bool is_err() const noexcept {
    return index() == RESULT_ERR_INDEX;
  }

  [[nodiscard]]
  inline json& data() const {
    if (is_err())
      throw std::runtime_error("tried getting data() from error result");

    return *(get<nlohmann::json*>(*this));
  }

  [[nodisard]]
  inline error err() const {
    if (is_ok())
      throw std::runtime_error("tried getting err() from data result");

    return get<error>(*this);
  }
};

}  // namespace luthor
