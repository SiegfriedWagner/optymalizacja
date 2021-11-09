project(optymalizacja)
# test euler
# add_executable(test_euler test_euler.cpp)
#  target_link_libraries(test_euler gtest gmock libgraph libgraph_euler)
add_test(NAME test_euler COMMAND test_euler)
# test matching
# add_executable(test_matching test_matching.cpp)
# target_link_libraries(test_matching gtest gmock libgraph libgraph_matching)
add_test(NAME test_matching COMMAND test_matching)
# test btree
# add_executable(test_btree ../src/BinaryTree.h test_btree.cpp)
# target_link_libraries(test_btree gtest gmock)
add_test(NAME test_btree COMMAND test_btree)
# all tests
add_executable(Google_Tests_run test_euler.cpp test_matching.cpp test_btree.cpp)
target_link_libraries(Google_Tests_run gtest gmock gtest_main)
# this looks bad
target_link_libraries(Google_Tests_run gtest gmock libgraph libgraph_matching)
target_link_libraries(Google_Tests_run gmock libgraph libgraph_euler)
