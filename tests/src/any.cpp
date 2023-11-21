#include "luthor_testing_pre.hpp"

TEST_CASE("any") {
  __TEST_CASE_PRE__;

  SECTION("default (meta)") {
    auto validator = l::any();
    json* pointer = nullptr;

    PASSING(_STRING);
    PASSING(_FLOAT);
    PASSING(_INTEGER);
    PASSING(_BOOLEAN);
    PASSING(_NULL);
    PASSING(_OBJECT);
    PASSING(_ARRAY);

    FAILING_NULL_AS(_DOES_NOT_EXIST, l::key_not_found_error);
  }

  SECTION("copy (meta)") {
    l::any::validator_f f = [](json& obj) -> void {
      (void)obj;
    };

    auto a = l::any().optional().nullable().custom_validator(f);
    auto b = l::any(a);

    REQUIRE(a.m_flags == b.m_flags);
    REQUIRE(a.m_validators.size() == b.m_validators.size());
  }

  SECTION("move (meta)") {
    l::any::validator_f f = [](json& obj) -> void {
      (void)obj;
    };

    auto a = l::any().optional().nullable().custom_validator(f);
    auto b =
      l::any(std::move(l::any().optional().nullable().custom_validator(f)));

    REQUIRE(a.m_flags == b.m_flags);
    REQUIRE(a.m_validators.size() == b.m_validators.size());
  }

  SECTION("optional") {
    auto validator = l::any().optional();
    json* pointer = nullptr;

    PASSING(_STRING);
    PASSING(_FLOAT);
    PASSING(_INTEGER);
    PASSING(_BOOLEAN);
    PASSING(_NULL);
    PASSING(_OBJECT);
    PASSING(_ARRAY);

    PASSING_NULL(_DOES_NOT_EXIST);
  }

  SECTION("nullable") {
    auto validator = l::any().nullable();
    json* pointer = nullptr;

    PASSING(_STRING);
    PASSING(_FLOAT);
    PASSING(_INTEGER);
    PASSING(_BOOLEAN);
    PASSING(_NULL);
    PASSING(_OBJECT);
    PASSING(_ARRAY);

    FAILING_NULL_AS(_DOES_NOT_EXIST, l::key_not_found_error);
  }

  SECTION("custom_validator") {
    std::size_t access_counter = 0;
    l::any::validator_f f = [&](json& obj) -> void {
      (void)obj;
      access_counter++;
    };

    auto validator = l::any().custom_validator(f);
    json* pointer = nullptr;

    REQUIRE(access_counter == 0);
    // always 3 sub-calls per PASSING(...) macro call

    PASSING(_STRING);
    REQUIRE(access_counter == 3);

    PASSING(_FLOAT);
    REQUIRE(access_counter == 6);

    PASSING(_INTEGER);
    REQUIRE(access_counter == 9);

    PASSING(_BOOLEAN);
    REQUIRE(access_counter == 12);

    PASSING(_NULL);
    REQUIRE(access_counter == 15);

    PASSING(_OBJECT);
    REQUIRE(access_counter == 18);

    PASSING(_ARRAY);
    REQUIRE(access_counter == 21);

    FAILING_NULL_AS(_DOES_NOT_EXIST, l::key_not_found_error);
  }
}
