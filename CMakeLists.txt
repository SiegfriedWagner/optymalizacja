cmake_minimum_required(VERSION 3.16.3)
project(optymalizacja)
set(CMAKE_CXX_STANDARD 20)

# Download and unpack googletest at configure time
configure_file(GoogleTest.txt googletest-download/CMakeLists.txt)
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
add_library(libgraph STATIC src/Digraph.h src/Digraph.cpp src/Graph.h src/IOHelpers.h src/std_extensions.h src/MatrixGraph.h src/MatrixGraph.cpp)
add_library(libgraph_bipariate STATIC src/Bipartiate.h src/Bipartiate.cpp)
add_library(libgraph_euler STATIC src/EulerCycle.h src/EulerCycle.cpp)
add_library(libgraph_matching STATIC src/Matching.h src/Matching.cpp)
add_library(libgraph_hungarian STATIC src/Hungarian.h src/Hungarian.cpp)
add_library(libgraph_converters STATIC src/GraphConverters.h)
add_library(libgraph_color STATIC src/GraphColoring.h src/GraphColoring.cpp)

# Tests
add_subdirectory(tests)

# Script executables
set(ARGPARSE external/argparse/argparse.h)
# link libs
target_link_libraries(libgraph_euler libgraph)
target_link_libraries(libgraph_bipariate libgraph)
target_link_libraries(libgraph_matching libgraph libgraph_bipariate)
target_link_libraries(libgraph_hungarian libgraph libgraph_bipariate libgraph_matching)
target_link_libraries(libgraph_converters libgraph)

# euler
add_executable(euler ${ARGPARSE} euler/main.cpp)
target_link_libraries(euler libgraph libgraph_euler)

# maximum matching
add_executable(matching ${ARGPARSE} maximum_matching/main.cpp)
target_link_libraries(matching libgraph libgraph_matching)

# hungarian
add_executable(hungarian ${ARGPARSE} hungarian/main.cpp)
target_link_libraries(hungarian libgraph libgraph_hungarian)

# tsp branch and bound
add_subdirectory(tsp_branch_and_bound)

# tsp mst
add_subdirectory(tsp_mst)

# graph coloring
add_subdirectory(graph_coloring)

#backpack
add_subdirectory(backpack)

# ants
add_subdirectory(ant_colony)