from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout
from conan.tools.files import copy
import git

class ECMConan(ConanFile):
    name = "ECM"
    version = "5.64.0"
    license = ""
    url = "ssh://git@192.168.105.40:7999/~manhpd9/extra-cmake-modules.git"
    description = "The Extra CMake Modules package,."
    generators = "CMakeToolchain", "CMakeDeps"
    settings = (
        "arch", 
        "build_type", 
        "compiler", 
        "os"
    )

    def source(self):
        git = tools.Git(folder="")
        git.clone(self.url)
        git.checkout("v%s"%self.version)

    def build(self):
        self.cmake = CMake(self)
        self.cmake.configure(source_folder="src")
        self.cmake.build()

    def package(self):
        self.cmake.install()

    def package_info(self):
        self.env_info.CMAKE_PREFIX_PATH.append(self.package_folder)
