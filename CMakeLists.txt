cmake_minimum_required(VERSION 3.17)
project(optymalizacja)
set(CMAKE_CXX_STANDARD 20)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

# Adds libraries
add_library(libgraph STATIC src/Digraph.h src/Digraph.cpp src/Graph.h src/Graph.cpp src/IOHelpers.h)
add_library(libgraph_euler STATIC src/EulerCycle.h src/EulerCycle.cpp)

# Tests
add_subdirectory(tests)

# Script executables
set(ARGPARSE external/argparse/argparse.h)
target_link_libraries(libgraph_euler libgraph)

# euler
add_executable(euler ${ARGPARSE} euler/main.cpp)
target_link_libraries(euler libgraph libgraph_euler)
