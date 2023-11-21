#pragma once

#include <fmt/core.h>

#include <string>
#include <type_traits>

#include "luthor/_comptime.hpp"
#include "luthor/_parseable.hpp"

namespace luthor {

class null : public parseable_i {
public:
  null() noexcept;
  ~null() noexcept = default;

  null(const null& other) noexcept = default;
  null(null&& other) noexcept = default;

  constexpr null& optional() noexcept {
    m_flags |= comptime::as_underlying_type(flags_e::optional);
    return *this;
  }

  [[nodiscard]]
  virtual inline std::unique_ptr<parseable_i> into_unique() noexcept override {
    return std::make_unique<null>(std::move(*this));
  }

  [[nodiscard]]
  virtual inline std::unique_ptr<parseable_i> copy_unique(
  ) const noexcept override {
    return std::make_unique<null>(*this);
  }

  virtual json* find_and_parse(json& parent, const std::string_view& key)
    const override;

  virtual json* parse(json& object) const override;

private:
  enum class flags_e {
    optional = 1 << 0,
  };

  std::underlying_type_t<flags_e> m_flags;
};

}  // namespace luthor
