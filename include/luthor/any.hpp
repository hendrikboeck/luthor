#pragma once

#include <fmt/core.h>

#include <string>
#include <type_traits>

#include "luthor/_comptime.hpp"
#include "luthor/_parseable.hpp"
#include "luthor/error.hpp"

namespace luthor {

class any : public parseable_i {
public:
  using validator_f = std::function<void(json&)>;

  any() noexcept;
  ~any() noexcept = default;

  any(const any& other) noexcept = default;
  any(const any&& other) noexcept;

  constexpr any& optional() noexcept {
    m_flags |= comptime::as_underlying_type(flags_e::optional);
    return *this;
  }

  constexpr any& nullable() noexcept {
    m_flags |= comptime::as_underlying_type(flags_e::nullable);
    return *this;
  }

  inline any& custom_validator(validator_f val) noexcept {
    m_validators.push_back(val);
    return *this;
  }

  [[nodiscard]]
  virtual inline std::unique_ptr<parseable_i> into_unique() noexcept override {
    return std::make_unique<any>(std::move(*this));
  }

  [[nodiscard]]
  virtual inline std::unique_ptr<parseable_i> copy_unique(
  ) const noexcept override {
    return std::make_unique<any>(*this);
  }

  virtual json* parse(json& object) const override;

  virtual json* find_and_parse(json& parent, const std::string_view& key)
    const override;

private:
  enum class flags_e {
    optional = 1 << 0,
    nullable = 1 << 1,
  };

  std::underlying_type_t<flags_e> m_flags;
  std::vector<validator_f> m_validators;
};

}  // namespace luthor
