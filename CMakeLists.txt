cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/StandardProjectSettings.cmake)
include(cmake/Sanitizers.cmake)
include(cmake/CompilerWarnings.cmake)
include(cmake/StaticAnalyzers.cmake)

project(ThinkCellAssigment)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(main main.cpp)
set_project_warnings(main)

add_executable(Tests tests.cpp tests.cpp)

target_include_directories(Tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(Tests PRIVATE ${CONAN_LIBS})

include(GoogleTest)
gtest_discover_tests(Tests)