cmake_minimum_required(VERSION 3.23)

option(SQLGEN_BUILD_SHARED "Build shared library" ${BUILD_SHARED_LIBS})

option(SQLGEN_SQLITE3 "Enable sqlite3 support" ON) # enabled by default

#option(SQLGEN_BUILD_BENCHMARKS "Build benchmarks" OFF)
#option(SQLGEN_BUILD_TESTS "Build tests" OFF)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SQLGEN_USE_VCPKG_DEFAULT ON)

if(SQLGEN_BUILD_BENCHMARKS)
    # TODO
endif()

option(SQLGEN_USE_VCPKG "Use VCPKG to download and build dependencies" ${SQLGEN_USE_VCPKG_DEFAULT})

if (SQLGEN_USE_VCPKG)
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")
endif ()

project(sqlgen VERSION 0.0.1 LANGUAGES CXX)

if (SQLGEN_BUILD_SHARED)
    add_library(sqlgen SHARED)
    set_target_properties(sqlgen PROPERTIES SOVERSION ${PROJECT_VERSION})
else()
    add_library(sqlgen STATIC)
endif()

if (MSVC)
    target_compile_options(sqlgen PRIVATE $<$<CONFIG:Debug>:-Wall>)
else()
    target_compile_options(sqlgen PRIVATE $<$<CONFIG:Debug>:-Wall -Wextra>)
endif()

set(SQLGEN_SOURCES
    src/sqlgen.cpp
)

target_include_directories(
    sqlgen PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

find_package(reflectcpp CONFIG REQUIRED)

set_target_properties(sqlgen PROPERTIES LINKER_LANGUAGE CXX)
target_sources(sqlgen PRIVATE ${SQLGEN_SOURCES})

if (SQLGEN_BUILD_TESTS)
    enable_testing()
    find_package(GTest CONFIG REQUIRED)
    set(SQLGEN_GTEST_LIB sqlgen GTest::gtest_main)
    add_subdirectory(tests)
endif ()

if (SQLGEN_BUILD_BENCHMARKS)
    if (NOT TARGET benchmark::benchmark)
        find_package(benchmark CONFIG REQUIRED)
    endif ()
    add_subdirectory(benchmarks)
endif ()

if (PROJECT_IS_TOP_LEVEL)
    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)

    configure_package_config_file(sqlgen-config.cmake.in
      ${CMAKE_CURRENT_BINARY_DIR}/sqlgen-config.cmake
      INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/sqlgen
      )

    install(
        FILES "${CMAKE_CURRENT_BINARY_DIR}/sqlgen-config.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/sqlgen"
    )

    file(GLOB_RECURSE RFL_HEADERS RELATIVE ${CMAKE_CURRENT_LIST_DIR} "${CMAKE_CURRENT_LIST_DIR}/include/*" )

    target_sources(sqlgen
        PUBLIC
        FILE_SET sqlgen_headers
        TYPE HEADERS
        BASE_DIRS $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        FILES ${RFL_HEADERS})

    install(
        TARGETS sqlgen
        EXPORT sqlgen-exports
        FILE_SET sqlgen_headers DESTINATION ${INCLUDE_INSTALL_DIR}
        )

    install(
        EXPORT sqlgen-exports
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/sqlgen
        NAMESPACE sqlgen::
    )
endif ()
