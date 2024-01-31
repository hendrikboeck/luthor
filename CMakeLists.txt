cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

# set(CMAKE_C_COMPILER gcc)
# set(CMAKE_CXX_COMPILER g++)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

project(
  luthor
  DESCRIPTION "luthor, a validation library for nlohmann/json"
  VERSION 0.0.1
  LANGUAGES CXX
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(PROJECT_VERSION "${PROJECT_VERSION}-debug")
endif()

option(SHARED "build shared library instead of static" OFF)

set(CMAKE_CXX_FLAGS "-Wno-error")

find_package(re2 REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(fmt REQUIRED)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

include_directories(include)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-attributes")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -DDEBUG -DSPDLOG_ACTIVE_LEVEL=1")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(SOURCES
  src/luthor_any.cpp
  src/luthor_array.cpp
  src/luthor_boolean.cpp
  src/luthor_error.cpp
  src/luthor_never.cpp
  src/luthor_null.cpp
  src/luthor_number.cpp
  src/luthor_object.cpp
  src/luthor_string.cpp
  src/luthor_variant.cpp
)
set(DEPENDENCIES
  re2::re2
  nlohmann_json::nlohmann_json
  fmt::fmt
)

if(BUILD_SHARED)

  add_library(${PROJECT_NAME}_shared SHARED ${SOURCES})
  target_include_directories(${PROJECT_NAME}_shared PUBLIC include)
  add_dependencies(${PROJECT_NAME}_shared ${DEPENDENCIES})
  target_link_libraries(${PROJECT_NAME}_shared ${DEPENDENCIES})
  #set_target_properties(${TARGET_NAME} PROPERTIES PUBLIC_HEADER "include/luthor.hpp")

endif()

add_library(${PROJECT_NAME}_static STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME}_static PUBLIC include)
add_dependencies(${PROJECT_NAME}_static ${DEPENDENCIES})
target_link_libraries(${PROJECT_NAME}_static ${DEPENDENCIES})
#set_target_properties(${TARGET_NAME} PROPERTIES PUBLIC_HEADER "include/luthor.hpp")

if(BUILD_SHARED)
  install(TARGETS ${PROJECT_NAME}_static ${PROJECT_NAME}_shared)
  else()
  install(TARGETS ${PROJECT_NAME}_static)
endif()

#
# --------------------- TESTING ---------------------
#
if(WITH_UNITTESTS)

find_package(Catch2 REQUIRED)

enable_testing()
add_test(NAME ${PROJECT_NAME}_test COMMAND ${PROJECT_NAME}_test)

include_directories(
  include
  tests/include
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")

add_executable(${PROJECT_NAME}_test
  tests/src/any.cpp
  tests/src/boolean.cpp
  tests/src/never.cpp
  tests/src/object.cpp
)
add_dependencies(${PROJECT_NAME}_test
  Catch2::Catch2WithMain
  ${PROJECT_NAME}_static
)
target_link_libraries(${PROJECT_NAME}_test
  Catch2::Catch2WithMain
  ${PROJECT_NAME}_static
)

endif()
