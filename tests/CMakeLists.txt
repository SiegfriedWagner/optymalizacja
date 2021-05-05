project(optymalizacja)
# test euler
add_executable(test_euler test_euler.cpp)
target_link_libraries(test_euler gtest gmock libgraph libgraph_euler)
add_test(NAME test_euler COMMAND test_euler)
# test matching
add_executable(test_matching test_matching.cpp)
target_link_libraries(test_matching gtest gmock libgraph libgraph_matching)
add_test(NAME test_matching COMMAND test_matching)
# test btree
add_executable(test_btree ../src/BinaryTree.h test_btree.cpp)
target_link_libraries(test_btree gtest gmock)
add_test(NAME test_btree COMMAND test_btree)
# test TSP
add_executable(test_tsp ../src/TSP.h test_tsp.cpp)
target_link_libraries(test_tsp gtest gmock libgraph)
add_test(NAME test_tsp COMMAND test_tsp)
