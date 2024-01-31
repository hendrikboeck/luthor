#pragma once

#include <iostream>
#include <tuple>
#include <unordered_map>
// #include <boost/unordered_map.hpp>

#include "luthor/_comptime.hpp"
#include "luthor/_parseable.hpp"

namespace luthor {

// template<typename K, typename V>
// using hashmap = boost::unordered_map<K, V>;

template<typename K, typename V>
using hashmap = std::unordered_map<K, V>;

class object : public parseable_i {
public:
  object() noexcept;
  ~object() noexcept = default;

  object(const object& other) noexcept;
  object(object&& other) noexcept;

  object& extend(const object& other);

  object& merge(object& other);

  // todo: only one layer at current moment, fix to support sub-objects
  // note: myb add functions deep_pick(...) instead
  object pick(const std::vector<std::string>& keys) const;

  // todo: only one layer at current moment, fix to support sub-objects
  // note: myb add functions deep_omit(...) instead
  object omit(const std::vector<std::string>& keys) const;

  inline object& item(const std::string& key, parseable_i& item) noexcept {
    m_items.insert_or_assign(key, item.into_unique());
    return *this;
  }

  inline object& item(const std::string& key, parseable_i&& item) noexcept {
    m_items.insert_or_assign(key, item.into_unique());
    return *this;
  }

  inline parseable_i* shape_of(const std::string& key) const {
    auto itr = m_items.find(key);

    if (itr == m_items.end())
      return nullptr;

    return itr->second.get();
  }

  constexpr object& optional() noexcept {
    m_flags |= comptime::as_underlying_type(flags_e::optional);
    return *this;
  }

  constexpr object& nullable() noexcept {
    m_flags |= comptime::as_underlying_type(flags_e::nullable);
    return *this;
  }

  constexpr object& partial() noexcept {
    m_flags |= comptime::as_underlying_type(flags_e::partial);
    return *this;
  }

  constexpr object& strict() noexcept {
    m_flags |= comptime::as_underlying_type(flags_e::strict);
    return *this;
  }

  inline object& catchall(parseable_i& value) noexcept {
    m_flags |= comptime::as_underlying_type(flags_e::catchall);
    m_catchall = value.into_unique();
    return *this;
  }

  constexpr object& strip() noexcept {
    m_flags |= comptime::as_underlying_type(flags_e::strip);
    return *this;
  }

  [[nodiscard]]
  virtual inline std::unique_ptr<parseable_i> into_unique() noexcept override {
    return std::make_unique<object>(std::move(*this));
  }

  [[nodiscard]]
  virtual inline std::unique_ptr<parseable_i> copy_unique(
  ) const noexcept override {
    return std::make_unique<object>(*this);
  }

  virtual json* find_and_parse(json& parent, const std::string_view& key)
    const override;

  virtual json* parse(json& object) const override;

private:
  enum class flags_e {
    // -- VALIDATE
    optional = 1 << 0,
    nullable = 1 << 1,
    partial = 1 << 2,
    strict = 1 << 3,
    catchall = 1 << 4,

    // -- TRANSFORM
    strip = 1 << 5,
  };

  std::underlying_type_t<flags_e> m_flags;
  hashmap<std::string, std::unique_ptr<parseable_i>> m_items;
  std::unique_ptr<parseable_i> m_catchall;

  inline object(
    std::underlying_type_t<flags_e> flags,
    hashmap<std::string, std::unique_ptr<parseable_i>> items,
    std::unique_ptr<parseable_i> catchall
  ) noexcept
      : m_flags(flags),
        m_items(std::move(items)),
        m_catchall(std::move(catchall)) { }
};

}  // namespace luthor
