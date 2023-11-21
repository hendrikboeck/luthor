#pragma once

#include <fmt/core.h>

#include <string>
#include <type_traits>

#include "luthor/_comptime.hpp"
#include "luthor/_parseable.hpp"
#include "luthor/error.hpp"

namespace luthor {

class boolean : public parseable_i {
public:
  boolean() noexcept;
  ~boolean() noexcept = default;

  boolean(const boolean& other) noexcept = default;
  boolean(boolean&& other) noexcept = default;

  constexpr boolean& optional() noexcept {
    m_flags |= comptime::as_underlying_type(flags_e::optional);
    return *this;
  }

  constexpr boolean& nullable() noexcept {
    m_flags |= comptime::as_underlying_type(flags_e::nullable);
    return *this;
  }

  constexpr boolean& literal(bool value) noexcept {
    m_flags |= comptime::as_underlying_type(flags_e::literal);
    m_literal = value;
    return *this;
  }

  [[nodiscard]]
  virtual inline std::unique_ptr<parseable_i> into_unique() noexcept override {
    return std::make_unique<boolean>(std::move(*this));
  }

  [[nodiscard]]
  virtual inline std::unique_ptr<parseable_i> copy_unique(
  ) const noexcept override {
    return std::make_unique<boolean>(*this);
  }

  virtual json* parse(json& object) const override;

  virtual json* find_and_parse(json& parent, const std::string_view& key)
    const override;

private:
  enum class flags_e {
    optional = 1 << 0,
    nullable = 1 << 1,
    literal = 1 << 2,
    // from_string = 1 << 3,
  };

  std::underlying_type_t<flags_e> m_flags;
  bool m_literal;
};

}  // namespace luthor
