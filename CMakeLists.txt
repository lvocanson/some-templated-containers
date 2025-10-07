cmake_minimum_required(VERSION 3.20)

project(some-templated-containers LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(stc INTERFACE)
target_include_directories(stc INTERFACE ${CMAKE_SOURCE_DIR}/include)
target_compile_features(stc INTERFACE cxx_std_20)

file(GLOB TEST_SOURCES
    ${CMAKE_SOURCE_DIR}/tests/*.cpp
)

add_executable(stc_tests ${TEST_SOURCES})
target_link_libraries(stc_tests PRIVATE stc)

enable_testing()
add_test(NAME stc_all_tests COMMAND stc_tests)
