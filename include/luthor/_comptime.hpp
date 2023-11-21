#pragma once

#include <cstdarg>
#include <string>
#include <type_traits>

namespace luthor::comptime {

template<typename T>
[[nodiscard]]
constexpr std::underlying_type_t<T> as_underlying_type(const T value) noexcept {
  return static_cast<std::underlying_type_t<T>>(value);
}

[[nodiscard]]
constexpr const char* bool_as_cstr(const bool value) noexcept {
  return value ? "true" : "false";
}

// from https://stackoverflow.com/a/46181/1550155
constexpr const char* EMAIL_REGEX =
  "(([^<>()[\\]\\.,;:\\s@\"]+(\\.[^<>()[\\]\\.,;:\\s@\"]+)*)|(\\\".+\\\"))"
  "@(([^<>()[\\]\\.,;:\\s@\\\"]+\\.)+[^<>()[\\]\\.,;:\\s@\\\"]{2,})";

constexpr const char* URL_REGEX =
  "https?:\\/\\/"
  "(?:www\\.)?[-a-zA-Z0-9@:%._\\+~#=]{1,256}"
  "\\.[a-zA-Z0-9()]{1,6}\\b(?:[-a-"
  "zA-Z0-9()@:%_\\+.~#?&\\/=]*)";

constexpr const char* UUID_REGEX =
  "[0-9a-fA-F]{8}\\b-[0-9a-fA-F]{4}\\b-[0-9a-"
  "fA-F]{4}\\b-[0-9a-fA-F]{4}\\b-["
  "0-9a-fA-F]{12}";

constexpr const char* CUID_REGEX = "c[^\\s-]{8,}";

constexpr const char* CUID2_REGEX = "[a-z][a-z0-9]*";

constexpr const char* ULID_REGEX = "[0-9A-HJKMNP-TV-Z]{26}";

constexpr const char* IPV4_REGEX =
  "(((25[0-5])|(2[0-4][0-9])|(1[0-9]{2})|([0-"
  "9]{1,2}))\\.){3}((25[0-5])|(2[0-"
  "4][0-9])|(1[0-9]{2})|([0-9]{1,2}))";

constexpr const char* IPV6_REGEX =
  "(([a-f0-9]{1,4}:){7}|::([a-f0-9]{1,4}:){0,"
  "6}|([a-f0-9]{1,4}:){1}:([a-f0-9]"
  "{1,4}:){0,5}|([a-f0-9]{1,4}:){2}:([a-f0-9]"
  "{1,4}:){0,4}|([a-f0-9]{1,4}:){3}"
  ":([a-f0-9]{1,4}:){0,3}|([a-f0-9]{1,4}:){4}"
  ":([a-f0-9]{1,4}:){0,2}|([a-f0-9]"
  "{1,4}:){5}:([a-f0-9]{1,4}:){0,1})([a-f0-9]"
  "{1,4}|(((25[0-5])|(2[0-4][0-9])|"
  "(1[0-9]{2})|([0-9]{1,2}))\\.){3}((25[0-5])"
  "|(2[0-4][0-9])|(1[0-9]{2})|([0-"
  "9]{1,2})))";

}  // namespace luthor::comptime
