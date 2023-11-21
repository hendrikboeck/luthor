#include "luthor/string.hpp"

namespace luthor {

using json = string::json;

/******************************************************************************/
/*                                                                            */
/*                                 PUBLIC                                     */
/*                                                                            */
/******************************************************************************/

string::string() noexcept
    : parseable_i(),
      m_flags(0),
      m_regex_type(regex_types_e::none),
      m_ll(0),
      m_ul(0),
      m_starts(),
      m_ends(),
      m_contains(),
      m_regex() { }

string::string(string&& other) noexcept
    : parseable_i(),
      m_flags(other.m_flags),
      m_regex_type(other.m_regex_type),
      m_ll(other.m_ll),
      m_ul(other.m_ul),
      m_starts(std::move(other.m_starts)),
      m_ends(std::move(other.m_ends)),
      m_contains(std::move(other.m_contains)),
      m_regex(std::move(other.m_regex)) { }

json* string::find_and_parse(json& parent, const std::string_view& key) const {
  auto itr = parent.find(key);

  if (itr == parent.end()) {
    if (m_flags & comptime::as_underlying_type(flags_e::optional))
      return nullptr;
    else
      throw error(fmt::format("key '{}' not found, but is required", key));
  }

  return parse(*itr);
}

json* string::parse(json& object) const {
  // base verification
  if (!object.is_string()) {
    if (m_flags & comptime::as_underlying_type(flags_e::nullable)) {
      if (object.is_null())
        return &object;
      else
        throw wrong_type_error("string or null");
    }

    throw wrong_type_error("string");
  }

  // convert to string reference
  auto& ref = object.get_ref<json::string_t&>();
  auto transform_before_parsing =
    m_flags & comptime::as_underlying_type(flags_e::transform_before_parsing);

  if (transform_before_parsing)
    apply_transform(ref);

  parse_bounds(ref);
  parse_basic(ref);
  parse_regex(ref);

  if (!transform_before_parsing)
    apply_transform(ref);

  return &object;
}

/******************************************************************************/
/*                                                                            */
/*                                 PRIVATE                                    */
/*                                                                            */
/******************************************************************************/

inline void string::apply_transform(std::string& value) const {
  // todo: probably needs optimization
  if (m_flags & comptime::as_underlying_type(flags_e::trim)) {
    // trim left
    value.erase(
      value.begin(),
      std::find_if(
        value.begin(), value.end(), [](uint8_t c) { return !std::isspace(c); }
      )
    );
    // trim right
    value.erase(
      std::find_if(
        value.rbegin(), value.rend(), [](uint8_t c) { return !std::isspace(c); }
      ).base(),
      value.end()
    );
  }

  if (m_flags & comptime::as_underlying_type(flags_e::to_upper_case))
    for (auto& c : value)
      c = toupper(c);

  else if (m_flags & comptime::as_underlying_type(flags_e::to_lower_case))
    for (auto& c : value)
      c = tolower(c);
}

inline void string::parse_bounds(std::string& value) const {
  auto len = value.length();

  if (m_flags & comptime::as_underlying_type(flags_e::min) && len < m_ll) {
    throw error(
      fmt::format("string object has to be min length of {}, is {}", m_ll, len)
    );
  }

  if (m_flags & comptime::as_underlying_type(flags_e::max) && len > m_ul) {
    throw error(
      fmt::format("string object has to be min length of {}, is {}", m_ul, len)
    );
  }
}

inline void string::parse_basic(std::string& value) const {
  if (!m_starts.empty() && !value.starts_with(m_starts))
    throw error(fmt::format("string object has to start with '{}'", m_starts));

  if (!m_ends.empty() && !value.ends_with(m_ends))
    throw error(fmt::format("string object has to end with '{}'", m_ends));
}

inline void string::parse_regex(std::string& value) const {
  switch (m_regex_type) {
  case regex_types_e::email :
    if (!RE2::FullMatch(value, RE2(comptime::EMAIL_REGEX)))
      throw error("failed to match email regex");
    break;

  case regex_types_e::url :
    if (!RE2::FullMatch(value, RE2(comptime::URL_REGEX)))
      throw error("failed to match url regex");
    break;

  case regex_types_e::uuid :
    if (!RE2::FullMatch(value, RE2(comptime::UUID_REGEX)))
      throw error("failed to match uuid regex");
    break;

  case regex_types_e::cuid :
    if (!RE2::FullMatch(value, RE2(comptime::CUID_REGEX)))
      throw error("failed to match cuid regex");
    break;

  case regex_types_e::cuid2 :
    if (!RE2::FullMatch(value, RE2(comptime::CUID2_REGEX)))
      throw error("failed to match cuid2 regex");
    break;

  case regex_types_e::ulid :
    if (!RE2::FullMatch(value, RE2(comptime::ULID_REGEX)))
      throw error("failed to match ulid regex");
    break;

  case regex_types_e::ipv4 :
    if (!RE2::FullMatch(value, RE2(comptime::IPV4_REGEX)))
      throw error("failed to match ipv4 regex");
    break;

  case regex_types_e::ipv6 :
    if (!RE2::FullMatch(value, RE2(comptime::IPV6_REGEX)))
      throw error("failed to match ipv6 regex");
    break;

  case regex_types_e::regex :
    if (!RE2::FullMatch(value, m_regex))
      throw error("failed to match custom regex");
    break;

  case regex_types_e::none :
  default :
    break;
  }
}

}  // namespace luthor
