#pragma once

#include <fmt/core.h>

#include <string>
#include <type_traits>
#include <variant>

#include "luthor/_comptime.hpp"
#include "luthor/_parseable.hpp"
#include "luthor/any.hpp"

namespace luthor {

class array : public parseable_i {
public:
  array() noexcept;
  ~array() noexcept = default;

  array(const array& other) noexcept;
  array(array&& other) noexcept;

  constexpr array& optional() noexcept {
    m_flags |= comptime::as_underlying_type(flags_e::optional);
    return *this;
  }

  constexpr array& nullable() noexcept {
    m_flags |= comptime::as_underlying_type(flags_e::nullable);
    return *this;
  }

  constexpr array& min(std::size_t value) noexcept {
    m_flags |= comptime::as_underlying_type(flags_e::min);
    m_ll = value;

    return *this;
  }

  constexpr array& max(std::size_t value) noexcept {
    m_flags |= comptime::as_underlying_type(flags_e::max);
    m_ul = value;

    return *this;
  }

  constexpr array& not_empty() noexcept {
    return min(1);
  }

  constexpr array& empty() noexcept {
    return max(0);
  }

  constexpr array& length(std::size_t value) noexcept {
    m_flags |= comptime::as_underlying_type(flags_e::min)
             | comptime::as_underlying_type(flags_e::max);
    m_ll = m_ul = value;

    return *this;
  }

  inline array& element(parseable_i& value) noexcept {
    m_flags |= comptime::as_underlying_type(flags_e::element);
    m_element = value.into_unique();

    return *this;
  }

  inline array& element(parseable_i&& value) noexcept {
    m_flags |= comptime::as_underlying_type(flags_e::element);
    m_element = value.into_unique();

    return *this;
  }

  [[nodiscard]]
  virtual inline std::unique_ptr<parseable_i> into_unique() noexcept override {
    return std::make_unique<array>(std::move(*this));
  }

  [[nodiscard]]
  virtual inline std::unique_ptr<parseable_i> copy_unique(
  ) const noexcept override {
    return std::make_unique<array>(*this);
  }

  virtual json* parse(json& object) const override;

  virtual json* find_and_parse(json& parent, const std::string_view& key)
    const override;

private:
  enum class flags_e {
    optional = 1 << 0,
    nullable = 1 << 1,
    min = 1 << 2,
    max = 1 << 3,
    element = 1 << 4,
  };

  std::underlying_type_t<flags_e> m_flags;
  std::unique_ptr<parseable_i> m_element;
  std::size_t m_ll, m_ul;
};

}  // namespace luthor
