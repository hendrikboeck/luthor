#include "luthor/never.hpp"

namespace luthor {

using json = never::json;

/******************************************************************************/
/*                                                                            */
/*                                 PUBLIC */
/*                                                                            */
/******************************************************************************/

json* never::find_and_parse(json& parent, const std::string_view& key) const {
  if (parent.find(key) != parent.end()) {
    throw error(fmt::format(
      "found an object for key '{}', but type never requires no object", key
    ));
  }

  return nullptr;
}

json* never::parse(json& object) const {
  (void)object;

  throw error("type is never, object should not exists");
}

/******************************************************************************/
/*                                                                            */
/*                                 PRIVATE */
/*                                                                            */
/******************************************************************************/

}  // namespace luthor
