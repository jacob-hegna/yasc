cmake_minimum_required (VERSION 2.6)
project (yasc)


set(SOURCES ./src/main.cpp ./src/parse.cpp)
set(SOURCES_TEST ./src/test.cpp ./src/parse.cpp)

# add the executable
add_executable (yasc      ${SOURCES})
add_executable (yasc-test ${SOURCES_TEST})

target_compile_options (yasc PUBLIC -std=c++17 -Wall -Werror -O3)
target_compile_options (yasc-test PUBLIC -std=c++17 -Wall -Werror -g)

target_link_libraries (yasc-test PUBLIC gtest gtest_main)
