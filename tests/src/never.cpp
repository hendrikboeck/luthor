#include "luthor_testing_pre.hpp"

TEST_CASE("never") {
  __TEST_CASE_PRE__;

  SECTION("default (meta)") {
    auto validator = l::never();
    json* pointer = nullptr;

    PASSING_NULL(_DOES_NOT_EXIST);

    FAILING_AS(_STRING, l::error);
    FAILING_AS(_FLOAT, l::error);
    FAILING_AS(_INTEGER, l::error);
    FAILING_AS(_BOOLEAN, l::error);
    FAILING_AS(_NULL, l::error);
    FAILING_AS(_OBJECT, l::error);
    FAILING_AS(_ARRAY, l::error);
  }
}
