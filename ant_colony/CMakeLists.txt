project(ant_colony)
add_executable(tsp_ant main.cpp)
target_link_libraries(tsp_ant libgraph)