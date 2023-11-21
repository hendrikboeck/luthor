#pragma once

#include <string>
#include <type_traits>
#include <vector>

#include "luthor/_comptime.hpp"
#include "luthor/_parseable.hpp"

namespace luthor {

class variant : public parseable_i {
public:
  variant() noexcept;
  ~variant() noexcept = default;

  variant(const variant& other) noexcept;
  variant(variant&& other) noexcept;

  inline variant& var(parseable_i& var) noexcept {
    m_variants.push_back(var.into_unique());
    return *this;
  }

  constexpr variant& optional() noexcept {
    m_flags |= comptime::as_underlying_type(flags_e::optional);
    return *this;
  }

  constexpr variant& nullable() noexcept {
    m_flags |= comptime::as_underlying_type(flags_e::nullable);
    return *this;
  }

  [[nodiscard]]
  virtual inline std::unique_ptr<parseable_i> into_unique() noexcept override {
    return std::make_unique<variant>(std::move(*this));
  }

  [[nodiscard]]
  virtual inline std::unique_ptr<parseable_i> copy_unique(
  ) const noexcept override {
    return std::make_unique<variant>(*this);
  }

  virtual json* find_and_parse(json& parent, const std::string_view& key)
    const override;

  virtual json* parse(json& object) const override;

private:
  enum class flags_e {
    optional = 1 << 0,
    nullable = 1 << 1,
  };

  std::underlying_type_t<flags_e> m_flags;
  std::vector<std::unique_ptr<parseable_i>> m_variants;
};

}  // namespace luthor
