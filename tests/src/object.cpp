#include "luthor_testing_pre.hpp"

TEST_CASE("object") {
  __TEST_CASE_PRE__;

  SECTION("default (meta)") {
    auto validator = l::object();
    json* pointer = nullptr;

    PASSING(_OBJECT);

    FAILING_AS(_STRING, l::wrong_type_error);
    FAILING_AS(_FLOAT, l::wrong_type_error);
    FAILING_AS(_INTEGER, l::wrong_type_error);
    FAILING_AS(_BOOLEAN, l::wrong_type_error);
    FAILING_AS(_NULL, l::wrong_type_error);
    FAILING_AS(_ARRAY, l::wrong_type_error);
    FAILING_NULL_AS(_DOES_NOT_EXIST, l::key_not_found_error);
  }

  SECTION("item") {
    auto alt = json({
      {"alt", object}
    });
    auto validator = l::object()
                       .item(_STRING, l::string())
                       .item(_FLOAT, l::number())
                       .item(_INTEGER, l::number().integer())
                       .item(_BOOLEAN, l::boolean())
                       .item(_NULL, l::null())
                       .item("never", l::never())
                       .item(_OBJECT, l::object())
                       .item(_ARRAY, l::array())
                       .item(_DOES_NOT_EXIST, l::any().optional());
    json* pointer = nullptr;

    PASSING_CUSTOM(alt, "alt");

    FAILING_AS(_OBJECT, l::error);
    FAILING_AS(_STRING, l::wrong_type_error);
    FAILING_AS(_FLOAT, l::wrong_type_error);
    FAILING_AS(_INTEGER, l::wrong_type_error);
    FAILING_AS(_BOOLEAN, l::wrong_type_error);
    FAILING_AS(_NULL, l::wrong_type_error);
    FAILING_AS(_ARRAY, l::wrong_type_error);
    FAILING_NULL_AS(_DOES_NOT_EXIST, l::key_not_found_error);
  }
}
