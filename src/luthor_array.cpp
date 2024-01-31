#include "luthor/array.hpp"

namespace luthor {

using json = array::json;

/******************************************************************************/
/*                                                                            */
/*                                 PUBLIC                                     */
/*                                                                            */
/******************************************************************************/

array::array() noexcept
    : parseable_i(),
      m_flags(0),
      m_element(nullptr),
      m_ll(0),
      m_ul(0) { }

array::array(const array& other) noexcept
    : parseable_i(),
      m_flags(other.m_flags),
      m_element(other.m_element ? other.m_element->copy_unique() : nullptr),
      m_ll(other.m_ll),
      m_ul(other.m_ul) { }

array::array(array&& other) noexcept
    : parseable_i(),
      m_flags(other.m_flags),
      m_element(std::move(other.m_element)),
      m_ll(other.m_ll),
      m_ul(other.m_ul) { }

json* array::find_and_parse(json& parent, const std::string_view& key) const {
  auto itr = parent.find(key);

  if (itr == parent.end()) {
    if (m_flags & comptime::as_underlying_type(flags_e::optional))
      return nullptr;
    else
      throw error(fmt::format("key '{}' not found, but is required", key));
  }

  return parse(*itr);
}

json* array::parse(json& object) const {
  if (!object.is_array()) {
    if (m_flags & comptime::as_underlying_type(flags_e::nullable) && object.is_null())
    {
      return &object;
    }

    throw error("object has to be type array");
  }

  auto& ref = object.get_ref<json::array_t&>();
  auto size = ref.size();

  if (m_flags & comptime::as_underlying_type(flags_e::min) && size < m_ll) {
    throw error(
      fmt::format("length of array has to be >= {}, but is {}", m_ll, size)
    );
  }

  if (m_flags & comptime::as_underlying_type(flags_e::max) && size > m_ul) {
    throw error(
      fmt::format("length of array has to be <= {}, but is {}", m_ul, size)
    );
  }

  if (m_flags & comptime::as_underlying_type(flags_e::element))
    for (auto& item : ref)
      m_element->parse(item);

  return &object;
}

/******************************************************************************/
/*                                                                            */
/*                                 PRIVATE                                    */
/*                                                                            */
/******************************************************************************/

}  // namespace luthor
