project(optymalizacja)
# test euler
add_executable(test_euler test_euler.cpp)
target_link_libraries(test_euler gtest gmock libgraph libgraph_euler)
add_test(NAME test_euler COMMAND test_euler)
# test matching
add_executable(test_matching test_matching.cpp)
target_link_libraries(test_matching gtest gmock libgraph libgraph_matching)
add_test(NAME test_matching COMMAND test_matching)

