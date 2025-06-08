cmake_minimum_required(VERSION 3.16)
set(HHGATEWAY_VERSION "1.0.0")
set(HHGATEWAY_SOVERSION "1.0.0")
project(
    HHGateway
    VERSION ${HHGATEWAY_VERSION}
    LANGUAGES CXX C
)
include(FeatureSummary)
find_package(ECM 6.14.0  NO_MODULE)

set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://commits.kde.org/extra-cmake-modules")
feature_summary(WHAT REQUIRED_PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)

set(CMAKE_MODULE_PATH
    ${ECM_MODULE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/
    ${ECM_KDE_MODULE_DIR}
    ${CMAKE_MODULE_PATH})


set(REQUIRED_QT_VERSION 6.7.0)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set build binary folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# set build library folder
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(MPDFRAMEWORK_VERSION "1.0.0")
set(MPDFRAMEWORK_SOVERSION "1")

option(QT6_BUILD "configure build to use Qt6/KF6" ON)
option(BUILD_TESTS "Build tests" OFF)

# testing options
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(BUILD_TESTS ON)
else()
    set(BUILD_TESTS OFF)
endif()

if(QT6_BUILD)
    set(QT_MAJOR_VERSION 6)
    set(QT_MIN_VERSION 6.7)
    set(KDDW_SUFFIX "-qt6")
else()
    set(QT_MAJOR_VERSION 5)
    set(QT_MIN_VERSION 5.15)
    set(KDDW_SUFFIX "")
endif()

find_package(
    Qt${QT_MAJOR_VERSION} ${QT_MIN_VERSION}
    COMPONENTS Core
               Widgets
               Network
               Test
    REQUIRED
)

#find_package(KF6ThreadWeaver NO_MODULE)
#find_package(
#     KF${QT_MAJOR_VERSION} 5.42.0
#     COMPONENTS ThreadWeaver
#     REQUIRED
# )

# logging
set(SPDLOG_BUILD_SHARED ON)
add_subdirectory(3rdparty/spdlog)
if(NOT TARGET spdlog)
    # Stand-alone build
    find_package(spdlog REQUIRED)
endif()

# unit test
set(BUILD_SHARED_LIBS ON)
add_subdirectory(3rdparty/googletest)
add_subdirectory(3rdparty/benchmark)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
    add_subdirectory(benchmarks)
endif()
add_subdirectory(3rdparty/threadweaver)
add_subdirectory(3rdparty/libpqxx)
# lib
add_subdirectory(src)

add_subdirectory(examples)
set(PAHO_BUILD_SHARED TRUE)
set(PAHO_WITH_SSL FALSE)
set(PAHO_WITH_MQTT_C FALSE)
add_subdirectory(3rdparty/paho.mqtt.cpp)
