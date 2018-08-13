cmake_minimum_required (VERSION 2.6)
project (yasc)

set(SOURCES ./src/main.cpp ./src/parser.cpp)
set(SOURCES_TEST ./src/test/test_arithmetic.cpp)

# add the executable
add_executable (yasc      ${SOURCES})
add_executable (yasc-test ${SOURCES_TEST})

target_compile_options (yasc      PUBLIC -std=c++17 -Wall -Werror -g -Og)
target_compile_options (yasc-test PUBLIC -std=c++17 -Wall -Werror -g)

target_link_libraries (yasc PUBLIC -lstdc++)
target_link_libraries (yasc-test PUBLIC -pthread gtest gtest_main)
