#pragma once

#include <fmt/core.h>

#include <string>

#include "luthor/_parseable.hpp"

namespace luthor {

class never : public parseable_i {
public:
  never() noexcept = default;
  ~never() noexcept = default;

  never(const never& other) noexcept = default;
  never(never&& other) noexcept = default;

  [[nodiscard]]
  virtual inline std::unique_ptr<parseable_i> into_unique() noexcept override {
    return std::make_unique<never>(std::move(*this));
  }

  [[nodiscard]]
  virtual inline std::unique_ptr<parseable_i> copy_unique(
  ) const noexcept override {
    return std::make_unique<never>(*this);
  }

  virtual json* find_and_parse(json& parent, const std::string_view& key)
    const override;

  virtual json* parse(json& object) const override;
};

}  // namespace luthor
