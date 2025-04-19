project(sqlgen-postgres-tests)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")

add_executable(
    sqlgen-postgres-tests 
    ${SOURCES}
)
target_precompile_headers(sqlgen-postgres-tests PRIVATE [["sqlgen.hpp"]] <iostream> <string> <functional> <gtest/gtest.h>)


target_link_libraries(
    sqlgen-postgres-tests
    PRIVATE 
    "${SQLGEN_GTEST_LIB}"
)

find_package(GTest)
gtest_discover_tests(sqlgen-postgres-tests)
