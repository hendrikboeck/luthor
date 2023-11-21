#include "luthor/variant.hpp"

namespace luthor {

using json = variant::json;

/******************************************************************************/
/*                                                                            */
/*                                 PUBLIC                                     */
/*                                                                            */
/******************************************************************************/

variant::variant() noexcept : parseable_i(), m_flags(0), m_variants() { }

variant::variant(const variant& other) noexcept
    : parseable_i(),
      m_flags(other.m_flags),
      m_variants(other.m_variants.size()) {
  for (const auto& var : other.m_variants)
    m_variants.push_back(var->copy_unique());
}

variant::variant(variant&& other) noexcept
    : parseable_i(),
      m_flags(other.m_flags),
      m_variants(std::move(other.m_variants)) { }

json* variant::find_and_parse(json& parent, const std::string_view& key) const {
  auto itr = parent.find(key);

  if (itr == parent.end()) {
    if (m_flags & comptime::as_underlying_type(flags_e::optional))
      return nullptr;
    else
      throw key_not_found_error(key);
  }

  return parse(*itr);
}

json* variant::parse(json& object) const {
  const bool option_nullable =
    m_flags & comptime::as_underlying_type(flags_e::nullable);

  if (option_nullable && object.is_null())
    return &object;

  bool passing = false;

  for (auto& var : m_variants) {
    try {
      var->parse(object);
      passing = true;
      break;
    }
    catch (const error& e) {
      (void)e;
    }
  }

  if (!passing) {
    if (option_nullable)
      throw error("no matching variant or null was found for object");
    throw error("no matching variant was found for object");
  }

  return &object;
}

/******************************************************************************/
/*                                                                            */
/*                                 PRIVATE                                    */
/*                                                                            */
/******************************************************************************/

}  // namespace luthor
