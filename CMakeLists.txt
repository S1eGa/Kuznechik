cmake_minimum_required(VERSION 3.20)
project(kuznechik)

set(CMAKE_BUILD_TYPE "Release")
# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)
add_subdirectory(lib)


add_executable(encrypt main.cpp)
target_link_libraries(encrypt kuznechik)

add_library(sse4_1 INTERFACE IMPORTED)
if(MSVC)
    target_compile_options(sse4_1 INTERFACE /arch:SSE4.1)
else()
    target_compile_options(sse4_1 INTERFACE -msse4.1)
endif()

enable_testing()

include(GoogleTest)

file(GLOB ALL_TESTS "${PROJECT_SOURCE_DIR}/test/*.cpp")

add_executable(tests
    ${ALL_TESTS}
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
target_link_libraries(
    tests
    kuznechik
    gtest
    gtest_main
)

gtest_discover_tests(tests)

