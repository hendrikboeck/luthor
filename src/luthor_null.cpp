#include "luthor/null.hpp"

namespace luthor {

using json = null::json;

/******************************************************************************/
/*                                                                            */
/*                                 PUBLIC                                     */
/*                                                                            */
/******************************************************************************/

null::null() noexcept : parseable_i(), m_flags(0) { }

json* null::find_and_parse(json& parent, const std::string_view& key) const {
  auto itr = parent.find(key);

  if (itr == parent.end()) {
    if (m_flags & comptime::as_underlying_type(flags_e::optional))
      return nullptr;
    else
      throw error(fmt::format("key '{}' not found, but is required", key));
  }

  return parse(*itr);
}

json* null::parse(json& object) const {
  if (!object.is_null())
    throw error("object has to be of type null");

  return &object;
}

/******************************************************************************/
/*                                                                            */
/*                                 PRIVATE                                    */
/*                                                                            */
/******************************************************************************/

}  // namespace luthor
