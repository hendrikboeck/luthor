#include "luthor/any.hpp"

namespace luthor {

using json = any::json;

/******************************************************************************/
/*                                                                            */
/*                                 PUBLIC                                     */
/*                                                                            */
/******************************************************************************/

any::any() noexcept : parseable_i(), m_flags(0), m_validators() { }

any::any(const any&& other) noexcept
    : parseable_i(),
      m_flags(other.m_flags),
      m_validators(std::move(other.m_validators)) { }

json* any::find_and_parse(json& parent, const std::string_view& key) const {
  auto itr = parent.find(key);

  if (itr == parent.end()) {
    if (m_flags & comptime::as_underlying_type(flags_e::optional))
      return nullptr;
    else
      throw key_not_found_error(key);
  }

  return parse(*itr);
}

json* any::parse(json& object) const {
  for (const auto& val : m_validators)
    val(object);

  return &object;
}

/******************************************************************************/
/*                                                                            */
/*                                 PRIVATE                                    */
/*                                                                            */
/******************************************************************************/

}  // namespace luthor
