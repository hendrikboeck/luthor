#pragma once

#include <fmt/core.h>

#include <string>
#include <type_traits>

#include "luthor/_comptime.hpp"
#include "luthor/_parseable.hpp"

namespace luthor {

class number : public parseable_i {
public:
  using integer_t = json::number_integer_t;
  using float_t = json::number_float_t;

  number() noexcept;
  ~number() noexcept = default;

  number(const number& other) noexcept = default;
  number(number&& other) noexcept = default;

  constexpr number& optional() noexcept {
    m_flags |= comptime::as_underlying_type(flags_e::optional);
    return *this;
  }

  constexpr number& nullable() noexcept {
    m_flags |= comptime::as_underlying_type(flags_e::nullable);
    return *this;
  }

  constexpr number& integer() noexcept {
    m_flags |= comptime::as_underlying_type(flags_e::integer);
    return *this;
  }

  constexpr number& gt(float_t value) noexcept {
    if (m_flags & comptime::as_underlying_type(flags_e::gte))
      m_flags ^= comptime::as_underlying_type(flags_e::gte);

    m_flags |= comptime::as_underlying_type(flags_e::gt);
    m_ll = value;

    return *this;
  }

  constexpr number& gte(float_t value) noexcept {
    if (m_flags & comptime::as_underlying_type(flags_e::gt))
      m_flags ^= comptime::as_underlying_type(flags_e::gt);

    m_flags |= comptime::as_underlying_type(flags_e::gte);
    m_ll = value;

    return *this;
  }

  constexpr number& positive() noexcept {
    return gt(static_cast<float_t>(0.0));
  }

  constexpr number& not_negative() noexcept {
    return gte(static_cast<float_t>(0.0));
  }

  constexpr number& min(float_t value) noexcept {
    return gte(value);
  }

  constexpr number& lt(float_t value) noexcept {
    if (m_flags & comptime::as_underlying_type(flags_e::lte))
      m_flags ^= comptime::as_underlying_type(flags_e::lte);

    m_flags |= comptime::as_underlying_type(flags_e::lt);
    m_ul = value;

    return *this;
  }

  constexpr number& lte(float_t value) noexcept {
    if (m_flags & comptime::as_underlying_type(flags_e::lt))
      m_flags ^= comptime::as_underlying_type(flags_e::lt);

    m_flags |= comptime::as_underlying_type(flags_e::lte);
    m_ul = value;

    return *this;
  }

  constexpr number& negative() noexcept {
    return lt(static_cast<float_t>(0.0));
  }

  constexpr number& not_positive() noexcept {
    return lte(static_cast<float_t>(0.0));
  }

  constexpr number& max(float_t value) noexcept {
    return lte(value);
  }

  [[nodiscard]]
  virtual inline std::unique_ptr<parseable_i> into_unique() noexcept override {
    return std::make_unique<number>(std::move(*this));
  }

  [[nodiscard]]
  virtual inline std::unique_ptr<parseable_i> copy_unique(
  ) const noexcept override {
    return std::make_unique<number>(*this);
  }

  virtual json* find_and_parse(json& parent, const std::string_view& key)
    const override;

  virtual json* parse(json& object) const override;

private:
  enum class flags_e {
    optional = 1 << 0,
    nullable = 1 << 1,
    integer = 1 << 2,
    lt = 1 << 3,
    lte = 1 << 4,
    gt = 1 << 5,
    gte = 1 << 6,
  };

  std::underlying_type_t<flags_e> m_flags;
  float_t m_ll, m_ul;

  inline json* parse_integer(json& object) const;
  inline json* parse_float(json& object) const;
};

}  // namespace luthor
