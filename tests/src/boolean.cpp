#include "luthor_testing_pre.hpp"

TEST_CASE("boolean") {
  __TEST_CASE_PRE__;

  LITERAL(_TRUE);
  LITERAL(_FALSE);

  object[_TRUE] = true;
  object[_FALSE] = false;

  SECTION("default (meta)") {
    auto validator = l::boolean();
    json* pointer = nullptr;

    PASSING(_BOOLEAN);

    FAILING_AS(_STRING, l::wrong_type_error);
    FAILING_AS(_FLOAT, l::wrong_type_error);
    FAILING_AS(_INTEGER, l::wrong_type_error);
    FAILING_AS(_NULL, l::wrong_type_error);
    FAILING_AS(_OBJECT, l::wrong_type_error);
    FAILING_AS(_ARRAY, l::wrong_type_error);
    FAILING_NULL_AS(_DOES_NOT_EXIST, l::key_not_found_error);
  }

  SECTION("copy (meta)") {
    auto a = l::boolean().optional().nullable().literal(true);
    auto b = l::boolean(a);

    REQUIRE(a.m_flags == b.m_flags);
    REQUIRE(a.m_literal == b.m_literal);
  }

  SECTION("move (meta)") {
    auto a = l::boolean().optional().nullable().literal(true);
    auto b =
      l::boolean(std::move(l::boolean().optional().nullable().literal(true)));

    REQUIRE(a.m_flags == b.m_flags);
    REQUIRE(a.m_literal == b.m_literal);
  }

  SECTION("optional") {
    auto validator = l::boolean().optional();
    json* pointer = nullptr;

    PASSING(_BOOLEAN);
    PASSING_NULL(_DOES_NOT_EXIST);

    FAILING_AS(_STRING, l::wrong_type_error);
    FAILING_AS(_FLOAT, l::wrong_type_error);
    FAILING_AS(_INTEGER, l::wrong_type_error);
    FAILING_AS(_NULL, l::wrong_type_error);
    FAILING_AS(_OBJECT, l::wrong_type_error);
    FAILING_AS(_ARRAY, l::wrong_type_error);
  }

  SECTION("nullable") {
    auto validator = l::boolean().nullable();
    json* pointer = nullptr;

    PASSING(_BOOLEAN);
    PASSING(_NULL);

    FAILING_AS(_STRING, l::wrong_type_error);
    FAILING_AS(_FLOAT, l::wrong_type_error);
    FAILING_AS(_INTEGER, l::wrong_type_error);
    FAILING_AS(_OBJECT, l::wrong_type_error);
    FAILING_AS(_ARRAY, l::wrong_type_error);
    FAILING_NULL_AS(_DOES_NOT_EXIST, l::key_not_found_error);
  }

  SECTION("literal") {
    auto validator = l::boolean().literal(true);
    json* pointer = nullptr;

    PASSING(_TRUE);

    FAILING_AS(_FALSE, l::error);
    FAILING_AS(_STRING, l::wrong_type_error);
    FAILING_AS(_FLOAT, l::wrong_type_error);
    FAILING_AS(_INTEGER, l::wrong_type_error);
    FAILING_AS(_NULL, l::wrong_type_error);
    FAILING_AS(_OBJECT, l::wrong_type_error);
    FAILING_AS(_ARRAY, l::wrong_type_error);
    FAILING_NULL_AS(_DOES_NOT_EXIST, l::key_not_found_error);

    validator.literal(false);

    PASSING(_FALSE);

    FAILING_AS(_TRUE, l::error);
    FAILING_AS(_STRING, l::wrong_type_error);
    FAILING_AS(_FLOAT, l::wrong_type_error);
    FAILING_AS(_INTEGER, l::wrong_type_error);
    FAILING_AS(_NULL, l::wrong_type_error);
    FAILING_AS(_OBJECT, l::wrong_type_error);
    FAILING_AS(_ARRAY, l::wrong_type_error);
    FAILING_NULL_AS(_DOES_NOT_EXIST, l::key_not_found_error);
  }
}
