#include "luthor/number.hpp"

namespace luthor {

using json = number::json;

/******************************************************************************/
/*                                                                            */
/*                                 PUBLIC                                     */
/*                                                                            */
/******************************************************************************/

number::number() noexcept : parseable_i(), m_flags(0), m_ll(0), m_ul(0) { }

json* number::find_and_parse(json& parent, const std::string_view& key) const {
  auto itr = parent.find(key);

  if (itr == parent.end()) {
    if (m_flags & comptime::as_underlying_type(flags_e::optional))
      return nullptr;
    else
      throw key_not_found_error(key);
  }

  return parse(*itr);
}

json* number::parse(json& object) const {
  switch (object.type()) {
  case json::value_t::number_float :
    if (m_flags & comptime::as_underlying_type(flags_e::integer))
      throw wrong_type_error("integer, but is float");
    return parse_float(object);

  case json::value_t::number_integer :
  case json::value_t::number_unsigned :
    if (m_flags & comptime::as_underlying_type(flags_e::integer))
      return parse_integer(object);
    return parse_float(object);

  case json::value_t::null :
    if (m_flags & comptime::as_underlying_type(flags_e::nullable))
      return &object;
    throw wrong_type_error("number");

  default :
    if (m_flags & comptime::as_underlying_type(flags_e::nullable))
      throw wrong_type_error("number or null");
    throw wrong_type_error("number");
  }
}

/******************************************************************************/
/*                                                                            */
/*                                 PRIVATE                                    */
/*                                                                            */
/******************************************************************************/

inline json* number::parse_integer(json& object) const {
  auto& ref = object.get_ref<json::number_integer_t&>();
  integer_t ll = static_cast<integer_t>(m_ll);
  integer_t ul = static_cast<integer_t>(m_ul);

  if (m_flags & comptime::as_underlying_type(flags_e::lt)) {
    if (ref >= ul)
      throw error(fmt::format("integer has to be < {}", ul));
  }

  else if (m_flags & comptime::as_underlying_type(flags_e::lte)) {
    if (ref > ul)
      throw error(fmt::format("integer has to be =< {}", ul));
  }

  if (m_flags & comptime::as_underlying_type(flags_e::gt)) {
    if (ref <= ll)
      throw error(fmt::format("integer has to be > {}", ll));
  }

  else if (m_flags & comptime::as_underlying_type(flags_e::gte)) {
    if (ref < ll)
      throw error(fmt::format("integer has to be >= {}", ll));
  }

  return &object;
}

inline json* number::parse_float(json& object) const {
  auto& ref = object.get_ref<json::number_float_t&>();

  if (m_flags & comptime::as_underlying_type(flags_e::lt)) {
    if (ref >= m_ul)
      throw error(fmt::format("integer has to be < {}", m_ul));
  }

  else if (m_flags & comptime::as_underlying_type(flags_e::lte)) {
    if (ref > m_ul)
      throw error(fmt::format("integer has to be =< {}", m_ul));
  }

  if (m_flags & comptime::as_underlying_type(flags_e::gt)) {
    if (ref <= m_ll)
      throw error(fmt::format("integer has to be > {}", m_ll));
  }

  else if (m_flags & comptime::as_underlying_type(flags_e::gte)) {
    if (ref < m_ll)
      throw error(fmt::format("integer has to be >= {}", m_ll));
  }

  return &object;
}

}  // namespace luthor
