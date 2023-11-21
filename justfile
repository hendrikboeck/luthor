set shell := ["sh", "-c"]

CC := "gcc"
CXX := "g++"
CMAKE_C_COMPILER := CC
CMAKE_CXX_COMPILER := CXX

alias pkg := package

conan-args := "-s compiler=gcc -s compiler.cppstd=23 -s compiler.version=13.1 -s compiler.libcxx=libstdc++11 -o with_unittests=True"

dev: dev-prebuild dev-build dev-test

dev-prebuild:
  CC="{{CC}}" CXX="{{CXX}}" conan install . -s build_type=Debug {{conan-args}} --build=missing
  cmake --preset conan-debug -DCMAKE_C_COMPILER={{CC}} -DCMAKE_CXX_COMPILER={{CXX}}

dev-build:
  cmake --build --preset conan-debug -- -j $(nproc)

dev-test:
  ./build/Debug/luthor_test

prod: prod-prebuild prod-build prod-test

prod-prebuild:
  CC="{{CC}}" CXX="{{CXX}}" conan install . -s build_type=Release {{conan-args}} --build=missing
  cmake --preset conan-release -DCMAKE_C_COMPILER={{CC}} -DCMAKE_CXX_COMPILER={{CXX}}

prod-build:
  cmake --build --preset conan-release -- -j $(nproc)

prod-test:
  cd ./build/Release && make test

format:
  clang-format -i src/*.cpp include/**/*.hpp test/*.cpp test/*.hpp

package:
  CC="{{CC}}" CXX="{{CXX}}" conan create . -s build_type=Release {{conan-args}} -o shared=True --build=missing

clean:
  rm -rf ./build CMakeUserPresets.json

