project(sqlgen-tests)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")

add_executable(
    sqlgen-tests 
    ${SOURCES}
)
target_precompile_headers(sqlgen-tests PRIVATE [["sqlgen.hpp"]] <iostream> <string> <functional> <gtest/gtest.h>)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std:c++20")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Werror -ggdb -fconcepts-diagnostics-depth=5")
endif()

target_link_libraries(
    sqlgen-tests  
    PRIVATE
    "${SQLGEN_GTEST_LIB}"
)

find_package(GTest)
gtest_discover_tests(sqlgen-tests)

if(SQLGEN_POSTGRES)
    add_subdirectory(postgres)
endif()

if(SQLGEN_SQLITE3)
    add_subdirectory(sqlite)
endif()
