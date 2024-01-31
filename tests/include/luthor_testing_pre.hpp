#pragma once

#include <catch2/catch_test_macros.hpp>
#include <nlohmann/json.hpp>

#define private public
#include "luthor.hpp"

#define STRINGIFY(str) #str

#define LITERAL(name) constexpr const char* name = STRINGIFY(name)

#define PASSING(literal)                                                       \
  REQUIRE_NOTHROW(validator.parse_safe(object[literal]).is_ok());              \
  REQUIRE_NOTHROW(pointer = validator.parse(object[literal]));                 \
  REQUIRE(pointer == &object[literal]);                                        \
  REQUIRE_NOTHROW(pointer = validator.find_and_parse(object, literal));        \
  REQUIRE(pointer == &object[literal]);

#define PASSING_CUSTOM(object, literal)                                        \
  REQUIRE_NOTHROW(validator.parse_safe(object[literal]).is_ok());              \
  REQUIRE_NOTHROW(pointer = validator.parse(object[literal]));                 \
  REQUIRE(pointer == &object[literal]);                                        \
  REQUIRE_NOTHROW(pointer = validator.find_and_parse(object, literal));        \
  REQUIRE(pointer == &object[literal]);

#define PASSING_NULL(literal)                                                  \
  REQUIRE_NOTHROW(pointer = validator.find_and_parse(object, literal));        \
  REQUIRE(pointer == nullptr);

/*
#define FAILING(literal)                                                       \
  REQUIRE_NOTHROW(validator.parse_safe(object[literal]).is_err());             \
  REQUIRE_THROWS_AS(validator.parse(object[literal]), l::error);               \
  REQUIRE_THROWS_AS(validator.find_and_parse(object, literal), l::error);
*/

#define FAILING_AS(literal, error)                                             \
  REQUIRE_NOTHROW(validator.parse_safe(object[literal]).is_err());             \
  REQUIRE_THROWS_AS(validator.parse(object[literal]), error);                  \
  REQUIRE_THROWS_AS(validator.find_and_parse(object, literal), error);

/*
#define FAILING_NULL(literal)                                                  \
  REQUIRE_THROWS_AS(validator.find_and_parse(object, literal), l::error);
*/

#define FAILING_NULL_AS(literal, error)                                        \
  REQUIRE_THROWS_AS(validator.find_and_parse(object, literal), error);

#define __TEST_CASE_PRE__                                                      \
  namespace l = luthor;                                                        \
  using json = nlohmann::json;                                                 \
                                                                               \
  LITERAL(_STRING);                                                            \
  LITERAL(_FLOAT);                                                             \
  LITERAL(_INTEGER);                                                           \
  LITERAL(_BOOLEAN);                                                           \
  LITERAL(_NULL);                                                              \
  LITERAL(_OBJECT);                                                            \
  LITERAL(_ARRAY);                                                             \
  LITERAL(_DOES_NOT_EXIST);                                                    \
                                                                               \
  auto object = json({                                                         \
    { _STRING,                     "undefined"},                                                  \
    {  _FLOAT,                            3.14},                                                          \
    {_INTEGER,                              42},                                                          \
    {_BOOLEAN,                            true},                                                        \
    {   _NULL,                         nullptr},                                                        \
    { _OBJECT, { { "subfield", "undefined" } }},                              \
    {  _ARRAY,                  { 0, 1, 2, 3 }},                                                \
  });
