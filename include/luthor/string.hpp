#pragma once

#include <fmt/core.h>
#include <re2/re2.h>

#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <regex>
#include <string>
#include <type_traits>

#include "luthor/_comptime.hpp"
#include "luthor/_parseable.hpp"
#include "luthor/error.hpp"

namespace luthor {

class string : public parseable_i {
public:
  string() noexcept;
  ~string() noexcept = default;

  string(const string& other) noexcept = default;
  string(string&& other) noexcept;

  constexpr string& optional() noexcept {
    m_flags |= comptime::as_underlying_type(flags_e::optional);
    return *this;
  }

  constexpr string& nullable() noexcept {
    m_flags |= comptime::as_underlying_type(flags_e::nullable);
    return *this;
  }

  constexpr string& trim() noexcept {
    m_flags |= comptime::as_underlying_type(flags_e::trim);
    return *this;
  }

  constexpr string& to_lower_case() noexcept {
    m_flags |= comptime::as_underlying_type(flags_e::to_lower_case);
    return *this;
  }

  constexpr string& to_upper_case() noexcept {
    m_flags |= comptime::as_underlying_type(flags_e::to_upper_case);
    return *this;
  }

  constexpr string& transform_before_parsing() noexcept {
    m_flags |= comptime::as_underlying_type(flags_e::transform_before_parsing);
    return *this;
  }

  // -- bounds
  constexpr string& min(size_t value) noexcept {
    m_flags |= comptime::as_underlying_type(flags_e::min);
    m_ll = value;
    return *this;
  }

  constexpr string& max(size_t value) noexcept {
    m_flags |= comptime::as_underlying_type(flags_e::max);
    m_ul = value;
    return *this;
  }

  constexpr string& length(size_t value) noexcept {
    m_flags |= comptime::as_underlying_type(flags_e::max)
             | comptime::as_underlying_type(flags_e::min);
    m_ul = m_ll = value;
    return *this;
  }

  constexpr string& not_empty() noexcept {
    m_flags |= comptime::as_underlying_type(flags_e::min);
    m_ll = 1;
    return *this;
  }

  // -- basic options
  inline string& starts_with(std::string value) noexcept {
    m_starts = value;
    return *this;
  }

  inline string& ends_with(std::string value) noexcept {
    m_ends = value;
    return *this;
  }

  inline string& contains(std::string value) noexcept {
    m_contains = value;
    return *this;
  }

  // -- regex options
  constexpr string& email() noexcept {
    m_regex_type = regex_types_e::email;
    return *this;
  }

  constexpr string& url() noexcept {
    m_regex_type = regex_types_e::url;
    return *this;
  }

  constexpr string& uuid() noexcept {
    m_regex_type = regex_types_e::uuid;
    return *this;
  }

  constexpr string& cuid() noexcept {
    m_regex_type = regex_types_e::cuid;
    return *this;
  }

  constexpr string& cuid2() noexcept {
    m_regex_type = regex_types_e::cuid2;
    return *this;
  }

  constexpr string& ulid() noexcept {
    m_regex_type = regex_types_e::ulid;
    return *this;
  }

  inline string& regex(std::string value) noexcept {
    m_regex_type = regex_types_e::regex;
    m_regex = value;
    return *this;
  }

  constexpr string& ipv4() noexcept {
    m_regex_type = regex_types_e::ipv4;
    return *this;
  }

  constexpr string& ipv6() noexcept {
    m_regex_type = regex_types_e::ipv6;
    return *this;
  }

  [[nodiscard]]
  virtual inline std::unique_ptr<parseable_i> into_unique() noexcept override {
    return std::make_unique<string>(std::move(*this));
  }

  [[nodiscard]]
  virtual inline std::unique_ptr<parseable_i> copy_unique(
  ) const noexcept override {
    return std::make_unique<string>(*this);
  }

  virtual json* find_and_parse(json& parent, const std::string_view& key)
    const override;

  virtual json* parse(json& object) const override;

private:
  enum class flags_e {
    // -- VALIDATE
    optional = 1 << 0,
    nullable = 1 << 1,
    transform_before_parsing = 1 << 2,
    min = 1 << 3,
    max = 1 << 4,

    // -- TRANSFORM
    trim = 1 << 5,
    to_lower_case = 1 << 6,
    to_upper_case = 1 << 7,
  };

  enum class regex_types_e {
    none,
    email,
    url,
    uuid,
    cuid,
    cuid2,
    ulid,
    ipv4,
    ipv6,
    regex,
  };

  inline void apply_transform(std::string& value) const;
  inline void parse_bounds(std::string& value) const;
  inline void parse_basic(std::string& value) const;
  inline void parse_regex(std::string& value) const;

  std::underlying_type_t<flags_e> m_flags;
  regex_types_e m_regex_type;

  // values of builder options
  std::size_t m_ll, m_ul;
  std::string m_starts, m_ends, m_contains, m_regex;
};

}  // namespace luthor
