cmake_minimum_required(VERSION 3.31)

project(snox-conversions
        VERSION 0.1.0
        DESCRIPTION "Conversion Library"
        HOMEPAGE_URL https://github.com/lennoxwestphal/snox-conversions
        LANGUAGES CXX
)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

add_library(snox-conversions INTERFACE)
add_library(snox::conversions ALIAS snox-conversions)

target_compile_features(snox-conversions INTERFACE cxx_std_23)

target_include_directories(snox-conversions INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

set(CMAKE_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/snox-conversions")

write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/snox-conversionsConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
        "cmake/snox-conversionsConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/snox-conversionsConfig.cmake"
        INSTALL_DESTINATION "${CMAKE_DIR}"
)

install(DIRECTORY include
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(TARGETS snox-conversions
        EXPORT snox-conversionsTargets)

install(EXPORT snox-conversionsTargets
        FILE snox-conversionsTargets.cmake
        NAMESPACE snox::
        DESTINATION "${CMAKE_DIR}")

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/snox-conversionsConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/snox-conversionsConfigVersion.cmake"
        DESTINATION "${CMAKE_DIR}")
