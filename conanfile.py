import os
import shutil

from conan import ConanFile
from conan.tools.scm import Git
from conan.tools.files import copy
from conan.tools.build import check_min_cppstd
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps


class luthorRecipe(ConanFile):
    name = "luthor"
    version = "0.1"
    package_type = "library"

    # Optional metadata
    license = "GPLv3"
    author = "Hendrik Böck <hendrikboeck.dev@protonmail.com>"
    url = "https://github.com/hendrikboeck/luthor.git"
    description = "simple JSON validation library for nlohmann/json, based on concepts from the JS library zod"
    topics = ("json")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "with_unittests": [True, False],
    }
    default_options = {
        "shared": False,
        "fPIC": False,
        "with_unittests": False,
    }

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "src/*", "include/*", "tests/*"

    generators = "CMakeDeps"

    # def source(self):
    #   git = Git(self)
    #   git.clone(url=f"{self.url}.git", target=".")

    def validate(self):
        check_min_cppstd(self, "20")

    def requirements(self):
        self.requires("nlohmann_json/3.11.2", transitive_headers=True)
        self.requires("re2/20230901", transitive_headers=True)
        self.requires("fmt/10.1.1", transitive_headers=True)
        self.requires("boost/1.83.0", transitive_headers=True)

        if self.options.with_unittests:
            self.requires(self.tested_reference_str)
            self.requires("catch2/3.4.0")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)

        if self.options.shared:
            tc.variables["BUILD_SHARED"] = True
            tc.variables["SHARED"] = True

        if self.options.with_unittests:
            tc.variables["WITH_UNITTESTS"] = True

        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)

        if self.options.with_unittests:
            cmake.test()

        cmake.install()
        shutil.copytree(os.path.join(self.source_folder, "include"),
                        os.path.join(self.package_folder, "include"))

    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", "luthor")
        self.cpp_info.set_property("cmake_target_name", "luthor::luthor")
        self.cpp_info.set_property("pkg_config_name", "luthor")

        self.cpp_info.libs = ["luthor_static"]
        if self.options.shared:
            self.cpp_info.libs.append("luthor_shared")
        self.cpp_info.libdirs = ["lib"]
        self.cpp_info.includedirs = ["include"]
