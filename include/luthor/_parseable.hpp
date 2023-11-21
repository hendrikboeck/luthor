#pragma once

#include <fmt/core.h>

#include <memory>
#include <nlohmann/json.hpp>
#include <string>

#include "luthor/_comptime.hpp"
#include "luthor/result.hpp"

namespace luthor {

class parseable_i {
public:
  using json = nlohmann::json;

  virtual json* find_and_parse(json& parent, const std::string_view& key)
    const = 0;

  virtual json* parse(json& object) const = 0;

  [[nodiscard]]
  virtual inline std::unique_ptr<parseable_i> into_unique() noexcept = 0;

  [[nodiscard]]
  virtual inline std::unique_ptr<parseable_i> copy_unique() const noexcept = 0;

  [[nodiscard]]
  inline result parse_safe(json& object) const noexcept {
    try {
      return *parse(object);
    }

    catch (const error& e) {
      return e;
    }

    catch (const json::exception& e) {
      return error(fmt::format(
        "caught nlohmann::json::exception! what(): {}", std::string(e.what())
      ));
    }

    catch (...) {
      return error("unkown error occured while parsing");
    }
  }
};

}  // namespace luthor
