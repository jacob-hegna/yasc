cmake_minimum_required (VERSION 2.6)
project (yasc)

set(SOURCES ./src/main.cpp ./src/parse.cpp)
set(SOURCES_TEST ./src/test/test_arithmetic.cpp)

# add the executable
add_executable (yasc      ${SOURCES})
add_executable (yasc-test ${SOURCES_TEST})

target_compile_options (yasc      PUBLIC -stdlib=libc++ -std=c++17 -Wall -Werror -O3)
target_compile_options (yasc-test PUBLIC -stdlib=libc++ -std=c++17 -Wall -Werror -g)

target_link_libraries (yasc PUBLIC -stdlib=libc++ -lstdc++)
target_link_libraries (yasc-test PUBLIC -stdlib=libc++ -pthread gtest gtest_main)
