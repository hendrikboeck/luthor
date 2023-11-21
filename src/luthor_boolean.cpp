#include "luthor/boolean.hpp"

namespace luthor {

using json = boolean::json;

/******************************************************************************/
/*                                                                            */
/*                                 PUBLIC                                     */
/*                                                                            */
/******************************************************************************/

boolean::boolean() noexcept : parseable_i(), m_flags(0), m_literal() { }

json* boolean::find_and_parse(json& parent, const std::string_view& key) const {
  auto itr = parent.find(key);

  if (itr == parent.end()) {
    if (m_flags & comptime::as_underlying_type(flags_e::optional))
      return nullptr;
    else
      throw key_not_found_error(key);
  }

  return parse(*itr);
}

json* boolean::parse(json& object) const {
  if (!object.is_boolean()) {
    if (m_flags & comptime::as_underlying_type(flags_e::nullable) && object.is_null())
    {
      return &object;
    }
    else {
      throw wrong_type_error("boolean or null");
    }

    throw wrong_type_error("boolean");
  }

  auto& ref = object.get_ref<json::boolean_t&>();

  if (m_flags & comptime::as_underlying_type(flags_e::literal) && ref != m_literal)
  {
    throw error(fmt::format(
      "boolean value is literal, should be {}, but is {}",
      comptime::bool_as_cstr(m_literal),
      comptime::bool_as_cstr(ref)
    ));
  }

  return &object;
}

/******************************************************************************/
/*                                                                            */
/*                                 PRIVATE                                    */
/*                                                                            */
/******************************************************************************/

}  // namespace luthor
