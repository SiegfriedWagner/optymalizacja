project(tsp_branch_and_bound)
add_executable(TSP_branchAndBound "../external/argparse/argparse.h" "main.cpp")
target_link_libraries(TSP_branchAndBound libgraph)

