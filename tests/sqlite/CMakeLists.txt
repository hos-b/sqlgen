project(sqlgen-sqlite-tests)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")

add_executable(
    sqlgen-sqlite-tests 
    ${SOURCES}
)
target_precompile_headers(sqlgen-sqlite-tests PRIVATE [["sqlgen.hpp"]] <iostream> <string> <functional> <gtest/gtest.h>)


target_link_libraries(
    sqlgen-sqlite-tests
    PRIVATE 
    "${SQLGEN_GTEST_LIB}"
)

find_package(GTest)
gtest_discover_tests(sqlgen-sqlite-tests)
