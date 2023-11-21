#include "luthor/object.hpp"

namespace luthor {

using json = object::json;

/******************************************************************************/
/*                                                                            */
/*                                 PUBLIC                                     */
/*                                                                            */
/******************************************************************************/

object::object() noexcept
    : parseable_i(),
      m_flags(0),
      m_items(),
      m_catchall(nullptr) { }

object::object(const object& other) noexcept
    : parseable_i(),
      m_flags(other.m_flags),
      m_items(),
      m_catchall(other.m_catchall->copy_unique()) {
  for (auto& kv : other.m_items)
    m_items.insert({ kv.first, kv.second->copy_unique() });
}

object::object(object&& other) noexcept
    : parseable_i(),
      m_flags(other.m_flags),
      m_items(std::move(other.m_items)),
      m_catchall(std::move(other.m_catchall)) { }

object& object::extend(const object& other) {
  for (auto& kv : other.m_items)
    m_items.insert_or_assign(kv.first, kv.second->copy_unique());

  return *this;
}

object& object::merge(object& other) {
  for (auto& kv : other.m_items)
    m_items.insert_or_assign(kv.first, std::move(kv.second));

  other.m_items = hashmap<std::string, std::unique_ptr<parseable_i>>();

  return *this;
}

object object::pick(const std::vector<std::string>& keys) const {
  auto new_items = hashmap<std::string, std::unique_ptr<parseable_i>>();

  for (auto& key : keys) {
    auto itr = m_items.find(key);
    if (itr != m_items.end())
      new_items.insert({ itr->first, itr->second->copy_unique() });
  }

  return object(m_flags, std::move(new_items), m_catchall->copy_unique());
}

object object::omit(const std::vector<std::string>& keys) const {
  object new_object = *this;

  for (auto& key : keys)
    new_object.m_items.erase(key);

  return new_object;
}

json* object::find_and_parse(json& parent, const std::string_view& key) const {
  auto itr = parent.find(key);

  if (itr == parent.end()) {
    if (m_flags & comptime::as_underlying_type(flags_e::optional))
      return nullptr;
    else
      throw key_not_found_error(key);
  }

  return parse(*itr);
}

json* object::parse(json& object) const {
  if (!object.is_object()) {
    if (m_flags & comptime::as_underlying_type(flags_e::nullable)) {
      if (object.is_null())
        return &object;

      throw wrong_type_error("object or null");
    }
    throw wrong_type_error("object");
  }

  const bool option_strip =
    m_flags & comptime::as_underlying_type(flags_e::strip);
  json tmp = {};

  for (const auto& kv : m_items) {
    const auto ptr = kv.second->find_and_parse(object, kv.first);
    if (option_strip && ptr != nullptr)
      tmp[kv.first] = *ptr;
  }

  if (option_strip)
    object = std::move(tmp);

  return &object;
}

/******************************************************************************/
/*                                                                            */
/*                                 PRIVATE                                    */
/*                                                                            */
/******************************************************************************/

}  // namespace luthor
